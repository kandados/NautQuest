#include "DesktopScreen.h"

#include <Arduino.h>
#include <lvgl.h>

#include "core/NavigationManager.h"
#include "core/ScreenManager.h"
#include "core/theme/ThemeManager.h"

static void settingsButtonEventHandler(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_CLICKED)
    {
        Serial.println("[Inicio] Ajustes pulsado");
        NQNavigation.goTo(ScreenID::Settings);
    }
}

static void developerButtonEventHandler(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_CLICKED)
    {
        Serial.println("[Inicio] Developer pulsado");
        NQNavigation.goTo(ScreenID::Developer);
    }
}

void DesktopScreen::show()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(lv_scr_act(), NQTheme.background(), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "NautQuest");
    lv_obj_set_style_text_color(title, NQTheme.text(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_42, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -155);

    lv_obj_t *missionsButton = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(missionsButton, NQTheme.primaryButton(), 0);
    lv_obj_set_size(missionsButton, 220, 45);
    lv_obj_align(missionsButton, LV_ALIGN_CENTER, 0, -45);
    lv_obj_add_event_cb(missionsButton, [](lv_event_t *event) {
        if (lv_event_get_code(event) == LV_EVENT_CLICKED)
        {
            Serial.println("[Inicio] Misiones pulsado");
            NQNavigation.goTo(ScreenID::Missions);
        }
    }, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *missionsLabel = lv_label_create(missionsButton);
    lv_label_set_text(missionsLabel, "Misiones");
    lv_obj_set_style_text_font(missionsLabel, &lv_font_montserrat_22, 0);
    lv_obj_center(missionsLabel);

    lv_obj_t *logbookButton = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(logbookButton, NQTheme.primaryButton(), 0);
    lv_obj_set_size(logbookButton, 220, 45);
    lv_obj_align(logbookButton, LV_ALIGN_CENTER, 0, 5);
    lv_obj_add_event_cb(logbookButton, [](lv_event_t *event) {
        if (lv_event_get_code(event) == LV_EVENT_CLICKED)
        {
            Serial.println("[Inicio] Bitacora pulsado");
            NQNavigation.goTo(ScreenID::Logbook);
        }
    }, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *logbookLabel = lv_label_create(logbookButton);
    lv_label_set_text(logbookLabel, "Bitacora");
    lv_obj_set_style_text_font(logbookLabel, &lv_font_montserrat_22, 0);
    lv_obj_center(logbookLabel);

    lv_obj_t *oracleButton = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(oracleButton, NQTheme.primaryButton(), 0);
    lv_obj_set_size(oracleButton, 220, 45);
    lv_obj_align(oracleButton, LV_ALIGN_CENTER, 0, 55);
    lv_obj_add_event_cb(oracleButton, [](lv_event_t *event) {
        if (lv_event_get_code(event) == LV_EVENT_CLICKED)
        {
            Serial.println("[Inicio] Oraculo pulsado");
            NQNavigation.goTo(ScreenID::Oracle);
        }
    }, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *oracleLabel = lv_label_create(oracleButton);
    lv_label_set_text(oracleLabel, "Oraculo");
    lv_obj_set_style_text_font(oracleLabel, &lv_font_montserrat_22, 0);
    lv_obj_center(oracleLabel);

    lv_obj_t *developerButton = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(developerButton, NQTheme.settingsButton(), 0);
    lv_obj_set_size(developerButton, 150, 38);
    lv_obj_align(developerButton, LV_ALIGN_CENTER, -80, 130);
    lv_obj_add_event_cb(developerButton, developerButtonEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *developerLabel = lv_label_create(developerButton);
    lv_label_set_text(developerLabel, "Developer");
    lv_obj_set_style_text_font(developerLabel, &lv_font_montserrat_16, 0);
    lv_obj_center(developerLabel);

    lv_obj_t *settingsButton = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(settingsButton, NQTheme.settingsButton(), 0);
    lv_obj_set_size(settingsButton, 150, 38);
    lv_obj_align(settingsButton, LV_ALIGN_CENTER, 80, 130);
    lv_obj_add_event_cb(settingsButton, settingsButtonEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *settingsLabel = lv_label_create(settingsButton);
    lv_label_set_text(settingsLabel, "Ajustes");
    lv_obj_set_style_text_font(settingsLabel, &lv_font_montserrat_16, 0);
    lv_obj_center(settingsLabel);

    lv_timer_handler();
}

DesktopScreen NQDesktop;