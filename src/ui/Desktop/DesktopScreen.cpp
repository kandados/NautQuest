#include "DesktopScreen.h"

#include <lvgl.h>

void DesktopScreen::show()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

    lv_obj_t *projectName = lv_label_create(lv_scr_act());
    lv_label_set_text(projectName, "NautQuest");
    lv_obj_set_style_text_color(projectName, lv_color_white(), 0);
    lv_obj_set_style_text_font(projectName, &lv_font_montserrat_48, 0);
    lv_obj_align(projectName, LV_ALIGN_CENTER, 0, -70);

    lv_obj_t *author = lv_label_create(lv_scr_act());
    lv_label_set_text(author, "By Kandadosdev 2026");
    lv_obj_set_style_text_color(author, lv_color_white(), 0);
    lv_obj_set_style_text_font(author, &lv_font_montserrat_22, 0);
    lv_obj_align(author, LV_ALIGN_CENTER, 0, -20);

    lv_obj_t *desktop = lv_label_create(lv_scr_act());
    lv_label_set_text(desktop, "DESKTOP");
    lv_obj_set_style_text_color(desktop, lv_color_white(), 0);
    lv_obj_set_style_text_font(desktop, &lv_font_montserrat_40, 0);
    lv_obj_align(desktop, LV_ALIGN_CENTER, 0, 55);

    lv_timer_handler();
}

DesktopScreen NQDesktop;