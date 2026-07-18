#include "NautyVideoPlayer.h"

#include <cstring>
#include <esp_heap_caps.h>

#include "storage/AssetManager.h"

NautyVideoPlayer NQNautyVideo;

extern NautyBehavior nautyBehavior;

namespace
{
    constexpr const char *IDLE_PATH =
        "/ui/nauty_video/idle.nqv";

    constexpr const char *HAPPY_PATH =
        "/ui/nauty_video/happy.nqv";

    constexpr const char *CURIOUS_PATH =
        "/ui/nauty_video/curious.nqv";

    uint16_t readLe16(const uint8_t *data)
    {
        return static_cast<uint16_t>(data[0]) |
            (static_cast<uint16_t>(data[1]) << 8);
    }
}

bool NautyVideoPlayer::begin(lv_obj_t *parent, const char *path)
{
    stop();

    if (parent == nullptr || path == nullptr)
    {
        Serial.println("[NautyVideo] ERROR: parent o ruta nulos");
        return false;
    }

    file_ = NQAssets.open(path);

    if (!file_ || !readHeader())
    {
        Serial.println("[NautyVideo] ERROR abriendo animacion");
        stop();
        return false;
    }

    frameBytes_ = static_cast<size_t>(width_) * height_ * 2;

    for (uint8_t index = 0; index < 2; ++index)
    {
        buffers_[index] = static_cast<uint8_t *>(
            heap_caps_malloc(
                frameBytes_,
                MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT
            )
        );

        if (buffers_[index] == nullptr)
        {
            Serial.println("[NautyVideo] ERROR reservando PSRAM");
            stop();
            return false;
        }

        descriptors_[index] = {};
        descriptors_[index].header.cf = LV_IMG_CF_TRUE_COLOR;
        descriptors_[index].header.always_zero = 0;
        descriptors_[index].header.reserved = 0;
        descriptors_[index].header.w = width_;
        descriptors_[index].header.h = height_;
        descriptors_[index].data_size = frameBytes_;
        descriptors_[index].data = buffers_[index];
    }

    if (!readFrame(0))
    {
        Serial.println("[NautyVideo] ERROR leyendo primer fotograma");
        stop();
        return false;
    }

    image_ = lv_img_create(parent);
    lv_img_set_src(image_, &descriptors_[0]);
    lv_obj_center(image_);
    lv_obj_clear_flag(image_, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(image_, LV_OBJ_FLAG_SCROLLABLE);

    visibleBuffer_ = 0;
    currentFrame_ = 1;
    displayedFrame_ = 0;
    playbackStartedAt_ = millis();
    statsStartedAt_ = playbackStartedAt_;
    renderedFrames_ = 1;
    renderedState_ = NautyState::IDLE;
    playing_ = true;

    Serial.printf(
        "[NautyVideo] Reproduciendo %ux%u, %u FPS, %u fotogramas\n",
        width_,
        height_,
        fps_,
        frameCount_
    );

    return true;
}

void NautyVideoPlayer::update()
{
    if (!playing_ || image_ == nullptr)
    {
        return;
    }

    syncBehaviorState();

    if (!playing_)
    {
        return;
    }

    const uint32_t now = millis();
    const uint32_t elapsedMs = now - playbackStartedAt_;
    const uint16_t targetFrame = static_cast<uint16_t>(
        (
            static_cast<uint64_t>(elapsedMs) * fps_ / 1000ULL
        ) % frameCount_
    );

    if (targetFrame == displayedFrame_)
    {
        return;
    }

    const uint8_t nextBuffer = visibleBuffer_ == 0 ? 1 : 0;

    /*
     * Si LVGL o la microSD se retrasan, avanzamos directamente al
     * fotograma que corresponde al reloj real. Así el movimiento no
     * se reproduce a cámara lenta: se descartan solamente los frames
     * que ya han vencido.
     */
    if (targetFrame != currentFrame_)
    {
        const size_t frameOffset =
            HEADER_BYTES +
            static_cast<size_t>(targetFrame) * frameBytes_;

        if (!file_.seek(frameOffset))
        {
            Serial.println("[NautyVideo] ERROR sincronizando animacion");
            stop();
            return;
        }
    }

    if (!readFrame(nextBuffer))
    {
        Serial.println("[NautyVideo] ERROR leyendo fotograma");
        stop();
        return;
    }

    showFrame(nextBuffer);
    visibleBuffer_ = nextBuffer;
    displayedFrame_ = targetFrame;
    currentFrame_ = targetFrame + 1;
    ++renderedFrames_;

    const uint32_t statsElapsedMs = now - statsStartedAt_;

    if (statsElapsedMs >= 5000)
    {
        const uint32_t fpsTimesTen =
            renderedFrames_ * 10000UL / statsElapsedMs;

        Serial.printf(
            "[NautyVideo] FPS reales: %lu.%lu\n",
            static_cast<unsigned long>(fpsTimesTen / 10),
            static_cast<unsigned long>(fpsTimesTen % 10)
        );

        statsStartedAt_ = now;
        renderedFrames_ = 0;
    }
}

void NautyVideoPlayer::stop()
{
    playing_ = false;

    if (image_ != nullptr)
    {
        lv_obj_del(image_);
        image_ = nullptr;
    }

    if (file_)
    {
        file_.close();
    }

    for (uint8_t index = 0; index < 2; ++index)
    {
        if (buffers_[index] != nullptr)
        {
            heap_caps_free(buffers_[index]);
            buffers_[index] = nullptr;
        }

        descriptors_[index] = {};
    }

    currentFrame_ = 0;
    displayedFrame_ = 0;
    frameBytes_ = 0;
    playbackStartedAt_ = 0;
    statsStartedAt_ = 0;
    renderedFrames_ = 0;
    renderedState_ = NautyState::IDLE;
}

bool NautyVideoPlayer::isPlaying() const
{
    return playing_;
}

bool NautyVideoPlayer::readHeader()
{
    uint8_t header[HEADER_BYTES];

    if (file_.read(header, sizeof(header)) != sizeof(header))
    {
        return false;
    }

    if (std::memcmp(header, "NQV1", 4) != 0)
    {
        return false;
    }

    width_ = readLe16(header + 4);
    height_ = readLe16(header + 6);
    fps_ = readLe16(header + 8);
    frameCount_ = readLe16(header + 10);

    return width_ > 0 &&
        width_ <= 466 &&
        height_ > 0 &&
        height_ <= 466 &&
        fps_ > 0 &&
        fps_ <= 30 &&
        frameCount_ > 0;
}

bool NautyVideoPlayer::switchAnimation(const char *path)
{
    if (path == nullptr || buffers_[0] == nullptr || buffers_[1] == nullptr)
    {
        return false;
    }

    File nextFile = NQAssets.open(path);

    if (!nextFile)
    {
        Serial.printf("[NautyVideo] No se pudo abrir: %s\n", path);
        return false;
    }

    uint8_t header[HEADER_BYTES];

    if (nextFile.read(header, sizeof(header)) != sizeof(header) ||
        std::memcmp(header, "NQV1", 4) != 0)
    {
        Serial.printf("[NautyVideo] Cabecera no valida: %s\n", path);
        nextFile.close();
        return false;
    }

    const uint16_t nextWidth = readLe16(header + 4);
    const uint16_t nextHeight = readLe16(header + 6);
    const uint16_t nextFps = readLe16(header + 8);
    const uint16_t nextFrameCount = readLe16(header + 10);

    if (nextWidth != width_ ||
        nextHeight != height_ ||
        nextFps == 0 ||
        nextFps > 30 ||
        nextFrameCount == 0)
    {
        Serial.printf("[NautyVideo] Formato incompatible: %s\n", path);
        nextFile.close();
        return false;
    }

    const uint8_t nextBuffer = visibleBuffer_ == 0 ? 1 : 0;

    if (nextFile.read(buffers_[nextBuffer], frameBytes_) != frameBytes_)
    {
        Serial.printf("[NautyVideo] Primer fotograma incompleto: %s\n", path);
        nextFile.close();
        return false;
    }

    if (file_)
    {
        file_.close();
    }

    file_ = nextFile;
    fps_ = nextFps;
    frameCount_ = nextFrameCount;

    showFrame(nextBuffer);
    visibleBuffer_ = nextBuffer;
    currentFrame_ = 1;
    displayedFrame_ = 0;

    playbackStartedAt_ = millis();
    statsStartedAt_ = playbackStartedAt_;
    renderedFrames_ = 1;

    Serial.printf(
        "[NautyVideo] Cambio a %s: %u FPS, %u fotogramas\n",
        path,
        fps_,
        frameCount_
    );

    return true;
}

void NautyVideoPlayer::syncBehaviorState()
{
    const NautyState behaviorState = nautyBehavior.getState();
    NautyState desiredState = NautyState::IDLE;

    if (behaviorState == NautyState::HAPPY)
    {
        desiredState = NautyState::HAPPY;
    }
    else if (behaviorState == NautyState::CURIOUS)
    {
        desiredState = NautyState::CURIOUS;
    }

    if (desiredState == renderedState_)
    {
        return;
    }

    const char *path = IDLE_PATH;

    if (desiredState == NautyState::HAPPY)
    {
        path = HAPPY_PATH;
    }
    else if (desiredState == NautyState::CURIOUS)
    {
        path = CURIOUS_PATH;
    }

    if (switchAnimation(path))
    {
        renderedState_ = desiredState;

        Serial.printf(
            "[NautyVideo] Estado visual: %s\n",
            nautyBehavior.getStateName()
        );
    }
    else
    {
        /* Evita repetir el mismo error en cada ciclo. */
        renderedState_ = desiredState;
    }
}

bool NautyVideoPlayer::readFrame(uint8_t bufferIndex)
{
    return file_.read(buffers_[bufferIndex], frameBytes_) == frameBytes_;
}

void NautyVideoPlayer::showFrame(uint8_t bufferIndex)
{
    lv_img_cache_invalidate_src(&descriptors_[bufferIndex]);
    lv_img_set_src(image_, &descriptors_[bufferIndex]);
    lv_obj_invalidate(image_);
}
