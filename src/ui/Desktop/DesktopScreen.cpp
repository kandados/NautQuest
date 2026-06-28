#include "DesktopScreen.h"

#include <lvgl.h>

void DesktopScreen::show() {
    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "NAUTQUEST OS");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_36, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -45);

    lv_obj_t *desktop = lv_label_create(lv_scr_act());
    lv_label_set_text(desktop, "DESKTOP");
    lv_obj_set_style_text_color(desktop, lv_color_white(), 0);
    lv_obj_set_style_text_font(desktop, &lv_font_montserrat_48, 0);
    lv_obj_align(desktop, LV_ALIGN_CENTER, 0, 25);

    lv_timer_handler();
}

DesktopScreen NQDesktop;