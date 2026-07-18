#pragma once

#include <Arduino.h>
#include <FS.h>
#include <lvgl.h>

#include "nauty/NautyBehavior.h"

class NautyVideoPlayer
{
public:
    bool begin(lv_obj_t *parent, const char *path);
    void update();
    void stop();

    bool isPlaying() const;

private:
    static constexpr size_t HEADER_BYTES = 12;

    bool readHeader();
    bool switchAnimation(const char *path);
    void syncBehaviorState();
    bool readFrame(uint8_t bufferIndex);
    void showFrame(uint8_t bufferIndex);

    File file_;
    lv_obj_t *image_ = nullptr;

    uint8_t *buffers_[2] = {nullptr, nullptr};
    lv_img_dsc_t descriptors_[2] = {};

    uint16_t width_ = 0;
    uint16_t height_ = 0;
    uint16_t fps_ = 0;
    uint16_t frameCount_ = 0;

    size_t frameBytes_ = 0;
    uint16_t currentFrame_ = 0;
    uint16_t displayedFrame_ = 0;
    uint8_t visibleBuffer_ = 0;

    uint32_t playbackStartedAt_ = 0;
    uint32_t statsStartedAt_ = 0;
    uint32_t renderedFrames_ = 0;

    NautyState renderedState_ = NautyState::IDLE;

    bool playing_ = false;
};

extern NautyVideoPlayer NQNautyVideo;
