#include "NQDisplay.h"

#include <Arduino.h>
#include <lvgl.h>
#include "Arduino_GFX_Library.h"

#include "drivers/touch/NQTouch.h"
#include "storage/AssetManager.h"
#include "assets/home/nautquest_boot_logo.h"

// Pines oficiales Waveshare ESP32-S3-Touch-AMOLED-1.75
#define LCD_SDIO0 4
#define LCD_SDIO1 5
#define LCD_SDIO2 6
#define LCD_SDIO3 7
#define LCD_SCLK 38
#define LCD_CS 12
#define LCD_RESET 39
#define LCD_WIDTH 466
#define LCD_HEIGHT 466

#define NQ_LVGL_TICK_PERIOD_MS 2

/*
 * Ruta LVGL de la imagen principal.
 *
 * La unidad S: representa la microSD registrada mediante
 * AssetManager.
 *
 * Ruta física en la tarjeta:
 *
 * /ui/home/nauty_home.bin
 */
static constexpr const char *NQ_HOME_IMAGE_PATH =
    "S:/ui/home/nauty_home.bin";

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LCD_WIDTH * LCD_HEIGHT / 10];

static Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    LCD_CS,
    LCD_SCLK,
    LCD_SDIO0,
    LCD_SDIO1,
    LCD_SDIO2,
    LCD_SDIO3
);

static Arduino_CO5300 *gfx = new Arduino_CO5300(
    bus,
    LCD_RESET,
    0,
    LCD_WIDTH,
    LCD_HEIGHT,
    6,
    0,
    0,
    0
);

static void nq_disp_flush(
    lv_disp_drv_t *disp,
    const lv_area_t *area,
    lv_color_t *color_p
)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

#if (LV_COLOR_16_SWAP != 0)
    gfx->draw16bitBeRGBBitmap(
        area->x1,
        area->y1,
        reinterpret_cast<uint16_t *>(&color_p->full),
        w,
        h
    );
#else
    gfx->draw16bitRGBBitmap(
        area->x1,
        area->y1,
        reinterpret_cast<uint16_t *>(&color_p->full),
        w,
        h
    );
#endif

    lv_disp_flush_ready(disp);
}

static void nq_lvgl_tick(void *arg)
{
    (void)arg;

    lv_tick_inc(NQ_LVGL_TICK_PERIOD_MS);
}

static void nq_touch_read(
    lv_indev_drv_t *driver,
    lv_indev_data_t *data
)
{
    NQTouch.read(driver, data);
}

void NautQuestDisplay::begin()
{
    Serial.print("[NQDisplay] begin start ms=");
    Serial.println(millis());

    Serial.print("[NQDisplay] gfx begin start ms=");
    Serial.println(millis());

    gfx->begin();

    Serial.print("[NQDisplay] gfx begin end ms=");
    Serial.println(millis());

    Serial.print("[NQDisplay] brightness start ms=");
    Serial.println(millis());

    gfx->setBrightness(200);

    Serial.print("[NQDisplay] brightness end ms=");
    Serial.println(millis());

    Serial.print("[NQDisplay] lvgl init start ms=");
    Serial.println(millis());

    lv_init();

    Serial.print("[NQDisplay] lvgl init end ms=");
    Serial.println(millis());

    /*
     * La microSD ya fue montada previamente por AssetManager
     * desde main.cpp.
     *
     * Ahora que LVGL está inicializado, registramos la unidad S:
     * para cargar imágenes mediante rutas como:
     *
     * S:/ui/home/nauty_home.bin
     */
    if (NQAssets.registerLvglFileSystem())
    {
        Serial.println(
            "[NQDisplay] microSD registrada en LVGL como S:"
        );
    }
    else
    {
        Serial.println(
            "[NQDisplay] ERROR registrando microSD en LVGL"
        );
    }

    lv_disp_draw_buf_init(
        &draw_buf,
        buf,
        nullptr,
        LCD_WIDTH * LCD_HEIGHT / 10
    );

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.flush_cb = nq_disp_flush;
    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register(&disp_drv);

    Serial.print("[NQDisplay] touch begin start ms=");
    Serial.println(millis());

    if (NQTouch.begin())
    {
        Serial.print("[NQDisplay] touch begin end ms=");
        Serial.println(millis());

        static lv_indev_drv_t indev_drv;
        lv_indev_drv_init(&indev_drv);

        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = nq_touch_read;

        lv_indev_drv_register(&indev_drv);

        Serial.println("[NQDisplay] Touch input registered");
    }
    else
    {
        Serial.print("[NQDisplay] touch begin failed ms=");
        Serial.println(millis());

        Serial.println(
            "[NQDisplay] Touch input not available"
        );
    }

    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &nq_lvgl_tick,
        .name = "nq_lvgl_tick"
    };

    esp_timer_handle_t lvgl_tick_timer = nullptr;

    esp_err_t timerCreateResult = esp_timer_create(
        &lvgl_tick_timer_args,
        &lvgl_tick_timer
    );

    if (timerCreateResult == ESP_OK)
    {
        esp_err_t timerStartResult = esp_timer_start_periodic(
            lvgl_tick_timer,
            NQ_LVGL_TICK_PERIOD_MS * 1000
        );

        if (timerStartResult != ESP_OK)
        {
            Serial.println(
                "[NQDisplay] ERROR iniciando temporizador LVGL"
            );
        }
    }
    else
    {
        Serial.println(
            "[NQDisplay] ERROR creando temporizador LVGL"
        );
    }

    Serial.print("[NQDisplay] AMOLED ready ms=");
    Serial.println(millis());
}

void NautQuestDisplay::showMascotBootScreen()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_black(),
        0
    );

    lv_obj_set_style_bg_opa(
        lv_scr_act(),
        LV_OPA_COVER,
        0
    );

    lv_obj_t *label = lv_label_create(
        lv_scr_act()
    );

    lv_label_set_text(
        label,
        " (^_^)\n"
    );

    lv_obj_set_style_text_color(
        label,
        lv_color_white(),
        0
    );

    lv_obj_set_style_text_font(
        label,
        &lv_font_montserrat_36,
        0
    );

    lv_obj_center(label);

    lv_timer_handler();
}

void NautQuestDisplay::showLogoBootScreen()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_black(),
        0
    );

    lv_obj_set_style_bg_opa(
        lv_scr_act(),
        LV_OPA_COVER,
        0
    );

    lv_obj_t *logo = lv_img_create(
        lv_scr_act()
    );

    /*
     * El logotipo de arranque continúa integrado en el firmware.
     *
     * Esto permite mostrarlo incluso cuando la microSD falta,
     * está dañada o todavía no se ha podido montar.
     */
    lv_img_set_src(
        logo,
        &nautquest_boot_logo
    );

    lv_obj_center(logo);

    lv_timer_handler();
}

void NautQuestDisplay::showAdventureBootScreen()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_black(),
        0
    );

    lv_obj_set_style_bg_opa(
        lv_scr_act(),
        LV_OPA_COVER,
        0
    );

    lv_obj_t *label = lv_label_create(
        lv_scr_act()
    );

    lv_label_set_text(
        label,
        "Comienza\n"
        "Tu aventura..."
    );

    lv_obj_set_style_text_color(
        label,
        lv_color_white(),
        0
    );

    lv_obj_set_style_text_font(
        label,
        &lv_font_montserrat_36,
        0
    );

    lv_obj_set_style_text_align(
        label,
        LV_TEXT_ALIGN_CENTER,
        0
    );

    lv_obj_center(label);

    lv_timer_handler();
}

void NautQuestDisplay::showHomeScreen()
{
    Serial.print("[NQDisplay] Loading Home image: ");
    Serial.println(NQ_HOME_IMAGE_PATH);

    lv_obj_clean(lv_scr_act());

    /*
     * Fondo negro para evitar destellos o restos de la pantalla
     * anterior mientras LVGL carga la imagen desde la microSD.
     */
    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_black(),
        0
    );

    lv_obj_set_style_bg_opa(
        lv_scr_act(),
        LV_OPA_COVER,
        0
    );

    lv_obj_t *homeImage = lv_img_create(
        lv_scr_act()
    );

    /*
     * LVGL reconoce que se trata de un archivo externo porque
     * recibe una ruta de texto en lugar de una variable de imagen
     * compilada dentro del firmware.
     */
    lv_img_set_src(
        homeImage,
        NQ_HOME_IMAGE_PATH
    );

    /*
     * La imagen se centra en la pantalla.
     *
     * Si el BIN mide 466 × 466 píxeles, ocupará exactamente
     * toda la superficie de la AMOLED.
     */
    lv_obj_center(homeImage);

    lv_timer_handler();

    Serial.println(
        "[NQDisplay] Home image requested from microSD"
    );
}

void NautQuestDisplay::update()
{
    lv_timer_handler();
}

NautQuestDisplay NQDisplay;