#include "MissionsScreen.h"

#include <Arduino.h>
#include <lvgl.h>

#include "core/MissionManager.h"
#include "core/NavigationManager.h"
#include "core/ScreenManager.h"
#include "core/theme/ThemeManager.h"

static void backButtonEventHandler(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[MissionsScreen] Volver pulsado");
        NQNavigation.goBack();
    }
}

static void openMissionEventHandler(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[MissionsScreen] Mision seleccionada");
        NQNavigation.goTo(ScreenID::MissionDetail);
    }
}

void MissionsScreen::show()
{
    Mission &mission = NQMission.activeMission();

    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(lv_scr_act(), NQTheme.background(), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "Misiones");
    lv_obj_set_style_text_color(title, NQTheme.text(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_40, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -145);

    lv_obj_t *subtitle = lv_label_create(lv_scr_act());
    lv_label_set_text(subtitle, "Selecciona una aventura");
    lv_obj_set_style_text_color(subtitle, NQTheme.text(), 0);
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_18, 0);
    lv_obj_align(subtitle, LV_ALIGN_CENTER, 0, -100);

    lv_obj_t *missionButton = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(missionButton, NQTheme.primaryButton(), 0);
    lv_obj_set_size(missionButton, 300, 70);
    lv_obj_align(missionButton, LV_ALIGN_CENTER, 0, -25);
    lv_obj_add_event_cb(missionButton, openMissionEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *missionLabel = lv_label_create(missionButton);
    lv_label_set_text(missionLabel, mission.title());
    lv_obj_set_style_text_font(missionLabel, &lv_font_montserrat_22, 0);
    lv_obj_center(missionLabel);

    char progressText[32];
    snprintf(progressText, sizeof(progressText), "Progreso: %d%%", mission.progress());

    lv_obj_t *progress = lv_label_create(lv_scr_act());
    lv_label_set_text(progress, progressText);
    lv_obj_set_style_text_color(progress, NQTheme.text(), 0);
    lv_obj_set_style_text_font(progress, &lv_font_montserrat_18, 0);
    lv_obj_align(progress, LV_ALIGN_CENTER, 0, 35);

    lv_obj_t *backButton = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(backButton, NQTheme.backButton(), 0);
    lv_obj_set_size(backButton, 150, 45);
    lv_obj_align(backButton, LV_ALIGN_CENTER, 0, 145);
    lv_obj_add_event_cb(backButton, backButtonEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *backLabel = lv_label_create(backButton);
    lv_label_set_text(backLabel, "Volver");
    lv_obj_set_style_text_font(backLabel, &lv_font_montserrat_20, 0);
    lv_obj_center(backLabel);

    lv_timer_handler();
}

MissionsScreen NQMissions;