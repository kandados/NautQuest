#include "BottomBarWidget.h"

#include <Arduino.h>

#include "core/NavigationManager.h"
#include "core/ScreenManager.h"
#include "core/theme/ThemeManager.h"

BottomBarWidget NQBottomBarWidget;

static void missionsEventHandler(lv_event_t* event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Misiones pulsado");
        NQNavigation.goTo(ScreenID::Missions);
    }
}

static void logbookEventHandler(lv_event_t* event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Bitacora pulsado");
        NQNavigation.goTo(ScreenID::Logbook);
    }
}

static void oracleEventHandler(lv_event_t* event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Oraculo pulsado");
        NQNavigation.goTo(ScreenID::Oracle);
    }
}

static lv_obj_t* createNavButton(
    lv_obj_t* parent,
    const char* text,
    lv_coord_t x,
    lv_event_cb_t handler
)
{
    lv_obj_t* button = lv_btn_create(parent);
    lv_obj_set_size(button, 112, 42);
    lv_obj_align(button, LV_ALIGN_CENTER, x, 0);
    lv_obj_set_style_radius(button, 16, 0);
    lv_obj_set_style_bg_color(button, NQTheme.primaryButton(), 0);
    lv_obj_add_event_cb(button, handler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t* label = lv_label_create(button);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
    lv_obj_center(label);

    return button;
}

lv_obj_t* BottomBarWidget::create(lv_obj_t* parent)
{
    lv_obj_t* container = lv_obj_create(parent);

    lv_obj_set_size(container, 390, 58);
    lv_obj_set_style_bg_opa(container, LV_OPA_0, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);

    createNavButton(container, "Misiones", -126, missionsEventHandler);
    createNavButton(container, "Bitacora", 0, logbookEventHandler);
    createNavButton(container, "IA", 126, oracleEventHandler);

    return container;
}
