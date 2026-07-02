#include "SettingsScreen.h"
#include "core/theme/ThemeManager.h"
#include <Arduino.h>
#include <lvgl.h>
#include "core/ScreenManager.h"
#include "core/NavigationManager.h"

static void backButtonEventHandler(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_CLICKED)
    {
        Serial.println("[SettingsScreen] Back button clicked");
        NQNavigation.goBack();
    }
}



void SettingsScreen::show()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "Configuracion");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_40, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -130);

    lv_obj_t *option1 = lv_label_create(lv_scr_act());
    lv_label_set_text(option1, "Pantalla");
    lv_obj_set_style_text_color(option1, lv_color_white(), 0);
    lv_obj_set_style_text_font(option1, &lv_font_montserrat_26, 0);
    lv_obj_align(option1, LV_ALIGN_CENTER, 0, -70);

    lv_obj_t *option2 = lv_label_create(lv_scr_act());
    lv_label_set_text(option2, "Sonido");
    lv_obj_set_style_text_color(option2, lv_color_white(), 0);
    lv_obj_set_style_text_font(option2, &lv_font_montserrat_26, 0);
    lv_obj_align(option2, LV_ALIGN_CENTER, 0, -25);

    lv_obj_t *option3 = lv_label_create(lv_scr_act());
    lv_label_set_text(option3, "Sistema");
    lv_obj_set_style_text_color(option3, lv_color_white(), 0);
    lv_obj_set_style_text_font(option3, &lv_font_montserrat_26, 0);
    lv_obj_align(option3, LV_ALIGN_CENTER, 0, 20);

    lv_obj_t *backButton = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(backButton, NQTheme.backButton(), 0);
    lv_obj_set_size(backButton, 180, 55);
    lv_obj_align(backButton, LV_ALIGN_CENTER, 0, 140
    );
    lv_obj_add_event_cb(backButton, backButtonEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *backLabel = lv_label_create(backButton);
    lv_label_set_text(backLabel, "Atras");
    lv_obj_set_style_text_font(backLabel, &lv_font_montserrat_24, 0);
    lv_obj_center(backLabel);

    lv_timer_handler();
}

SettingsScreen NQSettings;