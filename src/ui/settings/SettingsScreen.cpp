#include "SettingsScreen.h"

#include <lvgl.h>

void SettingsScreen::show()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "Settings");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_40, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -80);

    lv_obj_t *option1 = lv_label_create(lv_scr_act());
    lv_label_set_text(option1, "Display");
    lv_obj_set_style_text_color(option1, lv_color_white(), 0);
    lv_obj_set_style_text_font(option1, &lv_font_montserrat_28, 0);
    lv_obj_align(option1, LV_ALIGN_CENTER, 0, -20);

    lv_obj_t *option2 = lv_label_create(lv_scr_act());
    lv_label_set_text(option2, "Sound");
    lv_obj_set_style_text_color(option2, lv_color_white(), 0);
    lv_obj_set_style_text_font(option2, &lv_font_montserrat_28, 0);
    lv_obj_align(option2, LV_ALIGN_CENTER, 0, 35);

    lv_obj_t *option3 = lv_label_create(lv_scr_act());
    lv_label_set_text(option3, "System");
    lv_obj_set_style_text_color(option3, lv_color_white(), 0);
    lv_obj_set_style_text_font(option3, &lv_font_montserrat_28, 0);
    lv_obj_align(option3, LV_ALIGN_CENTER, 0, 90);

    lv_timer_handler();
}

SettingsScreen NQSettings;