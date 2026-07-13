#include "AssetManager.h"

#include <SD_MMC.h>
#include <cstring>
#include <esp_heap_caps.h>

AssetManager NQAssets;

bool AssetManager::begin()
{
    ready = false;
    lvglRegistered = false;

    Serial.println("[AssetManager] Inicializando microSD...");
    Serial.printf(
        "[AssetManager] Pines CLK=%d CMD=%d DATA=%d\n",
        SD_CLK_PIN,
        SD_CMD_PIN,
        SD_DATA_PIN
    );

    if (!SD_MMC.setPins(
        SD_CLK_PIN,
        SD_CMD_PIN,
        SD_DATA_PIN
    ))
    {
        Serial.println(
            "[AssetManager] ERROR configurando pines SDMMC"
        );

        return false;
    }

    if (!SD_MMC.begin("/sdcard", true))
    {
        Serial.println(
            "[AssetManager] ERROR montando la microSD"
        );

        return false;
    }

    const uint8_t cardType = SD_MMC.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println(
            "[AssetManager] No hay microSD insertada"
        );

        SD_MMC.end();
        return false;
    }

    ready = true;

    Serial.print("[AssetManager] Tipo: ");

    switch (cardType)
    {
        case CARD_MMC:
            Serial.println("MMC");
            break;

        case CARD_SD:
            Serial.println("SDSC");
            break;

        case CARD_SDHC:
            Serial.println("SDHC");
            break;

        default:
            Serial.println("Desconocido");
            break;
    }

    Serial.printf(
        "[AssetManager] Capacidad: %llu MB\n",
        totalBytes() / (1024ULL * 1024ULL)
    );

    Serial.printf(
        "[AssetManager] Usado: %llu MB\n",
        usedBytes() / (1024ULL * 1024ULL)
    );

    Serial.printf(
        "[AssetManager] Libre: %llu MB\n",
        freeBytes() / (1024ULL * 1024ULL)
    );

    Serial.printf(
        "[AssetManager] PSRAM detectada: %s\n",
        psramFound() ? "SI" : "NO"
    );

    if (psramFound())
    {
        Serial.printf(
            "[AssetManager] PSRAM libre: %u bytes\n",
            ESP.getFreePsram()
        );
    }

    Serial.println("[AssetManager] microSD lista");

    return true;
}

bool AssetManager::isReady() const
{
    return ready;
}

bool AssetManager::exists(const char *path) const
{
    if (!ready || path == nullptr || path[0] == '\0')
    {
        return false;
    }

    char normalizedPath[192];

    if (!normalizePath(path, normalizedPath, sizeof(normalizedPath)))
    {
        return false;
    }

    return SD_MMC.exists(normalizedPath);
}

File AssetManager::open(const char *path) const
{
    if (!ready)
    {
        Serial.println(
            "[AssetManager] microSD no disponible"
        );

        return File();
    }

    char normalizedPath[192];

    if (!normalizePath(path, normalizedPath, sizeof(normalizedPath)))
    {
        Serial.println(
            "[AssetManager] Ruta no valida"
        );

        return File();
    }

    File file = SD_MMC.open(normalizedPath, FILE_READ);

    if (!file)
    {
        Serial.print(
            "[AssetManager] No se pudo abrir: "
        );

        Serial.println(normalizedPath);
    }

    return file;
}

size_t AssetManager::fileSize(const char *path) const
{
    File file = open(path);

    if (!file)
    {
        return 0;
    }

    const size_t size = file.size();
    file.close();

    return size;
}

uint64_t AssetManager::totalBytes() const
{
    return ready ? SD_MMC.totalBytes() : 0;
}

uint64_t AssetManager::usedBytes() const
{
    return ready ? SD_MMC.usedBytes() : 0;
}

uint64_t AssetManager::freeBytes() const
{
    if (!ready)
    {
        return 0;
    }

    const uint64_t total = totalBytes();
    const uint64_t used = usedBytes();

    return total >= used ? total - used : 0;
}

bool AssetManager::registerLvglFileSystem()
{
    if (!ready)
    {
        Serial.println(
            "[AssetManager] No se puede registrar LVGL: microSD no disponible"
        );

        return false;
    }

    if (lvglRegistered)
    {
        return true;
    }

    lv_fs_drv_init(&fsDrv);

    fsDrv.letter = 'S';
    fsDrv.open_cb = fsOpen;
    fsDrv.close_cb = fsClose;
    fsDrv.read_cb = fsRead;
    fsDrv.seek_cb = fsSeek;
    fsDrv.tell_cb = fsTell;

    lv_fs_drv_register(&fsDrv);

    lvglRegistered = true;

    Serial.println(
        "[AssetManager] Sistema de archivos LVGL registrado como S:"
    );

    return true;
}

const lv_img_dsc_t *AssetManager::loadImageToPSRAM(
    const char *path
)
{
    if (!ready || path == nullptr || path[0] == '\0')
    {
        Serial.println(
            "[AssetManager] No se puede cargar imagen: ruta o SD no valida"
        );

        return nullptr;
    }

    for (size_t index = 0; index < MAX_CACHED_IMAGES; ++index)
    {
        if (
            imageCache[index].occupied &&
            imageCache[index].path == path
        )
        {
            return &imageCache[index].descriptor;
        }
    }

    size_t freeIndex = MAX_CACHED_IMAGES;

    for (size_t index = 0; index < MAX_CACHED_IMAGES; ++index)
    {
        if (!imageCache[index].occupied)
        {
            freeIndex = index;
            break;
        }
    }

    if (freeIndex == MAX_CACHED_IMAGES)
    {
        Serial.println(
            "[AssetManager] ERROR: caché de imágenes llena"
        );

        return nullptr;
    }

    File file = open(path);

    if (!file)
    {
        return nullptr;
    }

    const size_t fileBytes = file.size();

    if (fileBytes <= 4)
    {
        Serial.print(
            "[AssetManager] ERROR: BIN de imagen no valido: "
        );
        Serial.println(path);

        file.close();
        return nullptr;
    }

    uint8_t rawHeader[4];

    if (file.read(rawHeader, sizeof(rawHeader)) != sizeof(rawHeader))
    {
        Serial.print(
            "[AssetManager] ERROR leyendo cabecera: "
        );
        Serial.println(path);

        file.close();
        return nullptr;
    }

    const uint32_t headerValue =
        static_cast<uint32_t>(rawHeader[0]) |
        (static_cast<uint32_t>(rawHeader[1]) << 8) |
        (static_cast<uint32_t>(rawHeader[2]) << 16) |
        (static_cast<uint32_t>(rawHeader[3]) << 24);

    const uint32_t colorFormat = headerValue & 0x1F;
    const uint32_t alwaysZero = (headerValue >> 5) & 0x07;
    const uint32_t reserved = (headerValue >> 8) & 0x03;
    const uint32_t width = (headerValue >> 10) & 0x07FF;
    const uint32_t height = (headerValue >> 21) & 0x07FF;

    if (
        alwaysZero != 0 ||
        width == 0 ||
        height == 0
    )
    {
        Serial.print(
            "[AssetManager] ERROR: cabecera LVGL invalida: "
        );
        Serial.println(path);

        file.close();
        return nullptr;
    }

    const size_t pixelBytes = fileBytes - sizeof(rawHeader);

    uint8_t *pixelData = static_cast<uint8_t *>(
        heap_caps_malloc(
            pixelBytes,
            MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT
        )
    );

    if (pixelData == nullptr)
    {
        Serial.printf(
            "[AssetManager] ERROR reservando %u bytes en PSRAM\n",
            static_cast<unsigned>(pixelBytes)
        );

        file.close();
        return nullptr;
    }

    const size_t bytesRead = file.read(
        pixelData,
        pixelBytes
    );

    file.close();

    if (bytesRead != pixelBytes)
    {
        Serial.printf(
            "[AssetManager] ERROR leyendo %s: %u/%u bytes\n",
            path,
            static_cast<unsigned>(bytesRead),
            static_cast<unsigned>(pixelBytes)
        );

        heap_caps_free(pixelData);
        return nullptr;
    }

    CachedImage &entry = imageCache[freeIndex];

    entry.path = path;
    entry.pixelData = pixelData;
    entry.allocatedBytes = pixelBytes;
    entry.occupied = true;

    entry.descriptor.header.cf =
        static_cast<lv_img_cf_t>(colorFormat);

    entry.descriptor.header.always_zero = 0;
    entry.descriptor.header.reserved = reserved;
    entry.descriptor.header.w = width;
    entry.descriptor.header.h = height;
    entry.descriptor.data_size = pixelBytes;
    entry.descriptor.data = pixelData;

    Serial.printf(
        "[AssetManager] Imagen en PSRAM: %s (%ux%u, %u bytes)\n",
        path,
        static_cast<unsigned>(width),
        static_cast<unsigned>(height),
        static_cast<unsigned>(pixelBytes)
    );

    return &entry.descriptor;
}

void AssetManager::clearImageCache()
{
    for (size_t index = 0; index < MAX_CACHED_IMAGES; ++index)
    {
        CachedImage &entry = imageCache[index];

        if (entry.pixelData != nullptr)
        {
            heap_caps_free(entry.pixelData);
        }

        entry.path = "";
        entry.pixelData = nullptr;
        entry.descriptor = {};
        entry.allocatedBytes = 0;
        entry.occupied = false;
    }

    Serial.println(
        "[AssetManager] Caché de imágenes liberada"
    );
}

size_t AssetManager::cachedImageCount() const
{
    size_t count = 0;

    for (size_t index = 0; index < MAX_CACHED_IMAGES; ++index)
    {
        if (imageCache[index].occupied)
        {
            ++count;
        }
    }

    return count;
}

size_t AssetManager::cachedImageBytes() const
{
    size_t total = 0;

    for (size_t index = 0; index < MAX_CACHED_IMAGES; ++index)
    {
        total += imageCache[index].allocatedBytes;
    }

    return total;
}

bool AssetManager::normalizePath(
    const char *source,
    char *destination,
    size_t destinationSize
)
{
    if (
        source == nullptr ||
        destination == nullptr ||
        destinationSize < 2
    )
    {
        return false;
    }

    const char *path = source;

    if (
        path[0] != '\0' &&
        path[1] == ':'
    )
    {
        path += 2;
    }

    const int written = snprintf(
        destination,
        destinationSize,
        "%s%s",
        path[0] == '/' ? "" : "/",
        path
    );

    return (
        written > 0 &&
        static_cast<size_t>(written) < destinationSize
    );
}

void *AssetManager::fsOpen(
    lv_fs_drv_t *,
    const char *path,
    lv_fs_mode_t mode
)
{
    if ((mode & LV_FS_MODE_RD) == 0)
    {
        return nullptr;
    }

    char fullPath[192];

    if (!normalizePath(path, fullPath, sizeof(fullPath)))
    {
        return nullptr;
    }

    File *file = new File(
        SD_MMC.open(fullPath, FILE_READ)
    );

    if (!(*file))
    {
        Serial.print(
            "[AssetManager] LVGL no pudo abrir: "
        );

        Serial.println(fullPath);

        delete file;
        return nullptr;
    }

    return file;
}

lv_fs_res_t AssetManager::fsClose(
    lv_fs_drv_t *,
    void *filePointer
)
{
    if (filePointer == nullptr)
    {
        return LV_FS_RES_INV_PARAM;
    }

    File *file = static_cast<File *>(filePointer);

    file->close();
    delete file;

    return LV_FS_RES_OK;
}

lv_fs_res_t AssetManager::fsRead(
    lv_fs_drv_t *,
    void *filePointer,
    void *buffer,
    uint32_t bytesToRead,
    uint32_t *bytesRead
)
{
    if (
        filePointer == nullptr ||
        buffer == nullptr ||
        bytesRead == nullptr
    )
    {
        return LV_FS_RES_INV_PARAM;
    }

    File *file = static_cast<File *>(filePointer);

    *bytesRead = file->read(
        static_cast<uint8_t *>(buffer),
        bytesToRead
    );

    return LV_FS_RES_OK;
}

lv_fs_res_t AssetManager::fsSeek(
    lv_fs_drv_t *,
    void *filePointer,
    uint32_t position,
    lv_fs_whence_t whence
)
{
    if (filePointer == nullptr)
    {
        return LV_FS_RES_INV_PARAM;
    }

    File *file = static_cast<File *>(filePointer);

    SeekMode mode = SeekSet;

    switch (whence)
    {
        case LV_FS_SEEK_SET:
            mode = SeekSet;
            break;

        case LV_FS_SEEK_CUR:
            mode = SeekCur;
            break;

        case LV_FS_SEEK_END:
            mode = SeekEnd;
            break;

        default:
            return LV_FS_RES_INV_PARAM;
    }

    return file->seek(position, mode)
        ? LV_FS_RES_OK
        : LV_FS_RES_FS_ERR;
}

lv_fs_res_t AssetManager::fsTell(
    lv_fs_drv_t *,
    void *filePointer,
    uint32_t *position
)
{
    if (
        filePointer == nullptr ||
        position == nullptr
    )
    {
        return LV_FS_RES_INV_PARAM;
    }

    File *file = static_cast<File *>(filePointer);

    *position = file->position();

    return LV_FS_RES_OK;
}

bool AssetManager::buildPath(
    const char *directory,
    const char *filename,
    char *destination,
    size_t destinationSize
) const
{
    if (
        directory == nullptr ||
        filename == nullptr ||
        destination == nullptr ||
        destinationSize == 0
    )
    {
        return false;
    }

    if (directory[0] == '\0' || filename[0] == '\0')
    {
        destination[0] = '\0';
        return false;
    }

    const bool directoryEndsWithSlash =
        directory[strlen(directory) - 1] == '/';

    const bool filenameStartsWithSlash =
        filename[0] == '/';

    int written = 0;

    if (directoryEndsWithSlash && filenameStartsWithSlash)
    {
        written = snprintf(
            destination,
            destinationSize,
            "%s%s",
            directory,
            filename + 1
        );
    }
    else if (
        !directoryEndsWithSlash &&
        !filenameStartsWithSlash
    )
    {
        written = snprintf(
            destination,
            destinationSize,
            "%s/%s",
            directory,
            filename
        );
    }
    else
    {
        written = snprintf(
            destination,
            destinationSize,
            "%s%s",
            directory,
            filename
        );
    }

    if (
        written < 0 ||
        static_cast<size_t>(written) >= destinationSize
    )
    {
        destination[0] = '\0';

        Serial.println(
            "[AssetManager] Ruta demasiado larga"
        );

        return false;
    }

    return true;
}