#include "LogbookScreen.h"
#include "core/theme/ThemeManager.h"
#include <Arduino.h>
#include <lvgl.h>

#include "core/NavigationManager.h"

static void backButtonEventHandler(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[LogbookScreen] Volver pulsado");
        NQNavigation.goBack();
    }
}

void LogbookScreen::show()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "Bitacora");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_42, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -130);

    lv_obj_t *message = lv_label_create(lv_scr_act());
    lv_label_set_text(message,
                      "Aqui se guardaran\n"
                      "tus ideas,\n"
                      "hallazgos y avances.");
    lv_obj_set_style_text_color(message, lv_color_white(), 0);
    lv_obj_set_style_text_font(message, &lv_font_montserrat_22, 0);
    lv_obj_set_style_text_align(message, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(message, LV_ALIGN_CENTER, 0, -20);

    lv_obj_t *backButton = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(backButton, NQTheme.backButton(), 0);
    lv_obj_set_size(backButton, 170, 50);
    lv_obj_align(backButton, LV_ALIGN_CENTER, 0, 130);
    lv_obj_add_event_cb(backButton, backButtonEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *backLabel = lv_label_create(backButton);
    lv_label_set_text(backLabel, "Volver");
    lv_obj_set_style_text_font(backLabel, &lv_font_montserrat_22, 0);
    lv_obj_center(backLabel);

    lv_timer_handler();
}

LogbookScreen NQLogbook;