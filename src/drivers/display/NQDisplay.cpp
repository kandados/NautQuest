#include "NQDisplay.h"

#include <Arduino.h>
#include <lvgl.h>
#include "Arduino_GFX_Library.h"

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

static void nq_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

#if (LV_COLOR_16_SWAP != 0)
    gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

    lv_disp_flush_ready(disp);
}

static void nq_lvgl_tick(void *arg) {
    lv_tick_inc(NQ_LVGL_TICK_PERIOD_MS);
}

void NautQuestDisplay::begin() {
    Serial.println("[NQDisplay] Initializing AMOLED...");

    gfx->begin();
    gfx->setBrightness(200);

    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, LCD_WIDTH * LCD_HEIGHT / 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.flush_cb = nq_disp_flush;
    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register(&disp_drv);

    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &nq_lvgl_tick,
        .name = "nq_lvgl_tick"
    };

    esp_timer_handle_t lvgl_tick_timer = NULL;
    esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
    esp_timer_start_periodic(lvgl_tick_timer, NQ_LVGL_TICK_PERIOD_MS * 1000);

    Serial.println("[NQDisplay] AMOLED ready");
}

void NautQuestDisplay::showBootScreen() {
    lv_obj_clean(lv_scr_act());

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "NAUTQUEST");
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -30);

    lv_obj_t *subtitle = lv_label_create(lv_scr_act());
    lv_label_set_text(subtitle, "Booting...");
    lv_obj_align(subtitle, LV_ALIGN_CENTER, 0, 20);

    lv_timer_handler();
}

void NautQuestDisplay::update() {
    lv_timer_handler();
}

NautQuestDisplay NQDisplay;