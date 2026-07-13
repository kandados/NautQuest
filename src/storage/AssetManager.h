#pragma once

#include <Arduino.h>
#include <FS.h>
#include <lvgl.h>

class AssetManager
{
public:
    bool begin();
    bool isReady() const;

    bool exists(const char *path) const;
    File open(const char *path) const;
    size_t fileSize(const char *path) const;

    uint64_t totalBytes() const;
    uint64_t usedBytes() const;
    uint64_t freeBytes() const;

    bool registerLvglFileSystem();

    /*
     * Carga un archivo .bin de LVGL desde la microSD,
     * copia sus píxeles a PSRAM y devuelve un descriptor
     * que LVGL puede dibujar sin volver a leer la tarjeta.
     */
    const lv_img_dsc_t *loadImageToPSRAM(const char *path);

    /*
     * Libera todas las imágenes almacenadas en la caché.
     * Solo debe llamarse cuando los objetos LVGL que las usan
     * ya hayan sido destruidos.
     */
    void clearImageCache();

    size_t cachedImageCount() const;
    size_t cachedImageBytes() const;

    bool buildPath(
        const char *directory,
        const char *filename,
        char *destination,
        size_t destinationSize
    ) const;

    static constexpr const char *UI_HOME_PATH =
        "/ui/home";

    static constexpr const char *UI_LAUNCHER_PATH =
        "/ui/launcher";

    static constexpr const char *UI_LOGBOOK_PATH =
        "/ui/logbook";

    static constexpr const char *UI_MISSIONS_PATH =
        "/ui/missions";

    static constexpr const char *UI_ORACLE_PATH =
        "/ui/oracle";

    static constexpr const char *UI_SETTINGS_PATH =
        "/ui/settings";

    static constexpr const char *ICONS_PATH =
        "/icons";

    static constexpr const char *SOUNDS_PATH =
        "/sounds";

    static constexpr const char *ANIMATIONS_PATH =
        "/animations";

    static constexpr const char *MISSIONS_PATH =
        "/missions";

    static constexpr const char *THEMES_PATH =
        "/themes";

private:
    struct CachedImage
    {
        String path;
        uint8_t *pixelData = nullptr;
        lv_img_dsc_t descriptor = {};
        size_t allocatedBytes = 0;
        bool occupied = false;
    };

    static constexpr size_t MAX_CACHED_IMAGES = 8;

    bool ready = false;
    bool lvglRegistered = false;

    lv_fs_drv_t fsDrv;
    CachedImage imageCache[MAX_CACHED_IMAGES];

    static constexpr int SD_CLK_PIN = 2;
    static constexpr int SD_CMD_PIN = 1;
    static constexpr int SD_DATA_PIN = 3;

    static bool normalizePath(
        const char *source,
        char *destination,
        size_t destinationSize
    );

    static void *fsOpen(
        lv_fs_drv_t *drv,
        const char *path,
        lv_fs_mode_t mode
    );

    static lv_fs_res_t fsClose(
        lv_fs_drv_t *drv,
        void *file
    );

    static lv_fs_res_t fsRead(
        lv_fs_drv_t *drv,
        void *file,
        void *buf,
        uint32_t btr,
        uint32_t *br
    );

    static lv_fs_res_t fsSeek(
        lv_fs_drv_t *drv,
        void *file,
        uint32_t pos,
        lv_fs_whence_t whence
    );

    static lv_fs_res_t fsTell(
        lv_fs_drv_t *drv,
        void *file,
        uint32_t *pos
    );
};

extern AssetManager NQAssets;