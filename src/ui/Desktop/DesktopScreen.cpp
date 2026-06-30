
#include "core/MissionManager.h"
#include "DesktopScreen.h"
#include <Arduino.h>

#include <lvgl.h>

#include "core/ScreenManager.h"

static void settingsButtonEventHandler(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_CLICKED)
    {
        Serial.println("[DesktopScreen] Settings button clicked");
        NQScreen.show(ScreenID::Settings);
    }
}
void DesktopScreen::show()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

    lv_obj_t *projectName = lv_label_create(lv_scr_act());
    lv_label_set_text(projectName, "NautQuest");
    lv_obj_set_style_text_color(projectName, lv_color_white(), 0);
    lv_obj_set_style_text_font(projectName, &lv_font_montserrat_48, 0);
    lv_obj_align(projectName, LV_ALIGN_CENTER, 0, -90);

    lv_obj_t *author = lv_label_create(lv_scr_act());
    lv_label_set_text(author, "By Kandadosdev");
    lv_obj_set_style_text_color(author, lv_color_white(), 0);
    lv_obj_set_style_text_font(author, &lv_font_montserrat_22, 0);
    lv_obj_align(author, LV_ALIGN_CENTER, 0, -42);

    lv_obj_t *desktop = lv_label_create(lv_scr_act());
    lv_label_set_text(desktop, "DESKTOP");
    lv_obj_set_style_text_color(desktop, lv_color_white(), 0);
    lv_obj_set_style_text_font(desktop, &lv_font_montserrat_32, 0);
    lv_obj_align(desktop, LV_ALIGN_CENTER, 0, 15);

    lv_obj_t *missionTitle = lv_label_create(lv_scr_act());
    lv_label_set_text(missionTitle, "Mision activa");
    lv_obj_set_style_text_color(missionTitle, lv_color_white(), 0);
    lv_obj_set_style_text_font(missionTitle, &lv_font_montserrat_20, 0);
    lv_obj_align(missionTitle, LV_ALIGN_CENTER, 0, 55);

    lv_obj_t *missionText = lv_label_create(lv_scr_act());
    lv_label_set_text(missionText, NQMission.currentMission());
    lv_obj_set_style_text_color(missionText, lv_color_white(), 0);
    lv_obj_set_style_text_font(missionText, &lv_font_montserrat_22, 0);
    lv_obj_align(missionText, LV_ALIGN_CENTER, 0, 82);

    lv_obj_t *settingsButton = lv_btn_create(lv_scr_act());
    lv_obj_set_size(settingsButton, 190, 55);
    lv_obj_align(settingsButton, LV_ALIGN_CENTER, 0, 145);
    lv_obj_add_event_cb(settingsButton, settingsButtonEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *settingsLabel = lv_label_create(settingsButton);
    lv_label_set_text(settingsLabel, "Settings");
    lv_obj_set_style_text_font(settingsLabel, &lv_font_montserrat_24, 0);
    lv_obj_center(settingsLabel);

    lv_timer_handler();
}

DesktopScreen NQDesktop;