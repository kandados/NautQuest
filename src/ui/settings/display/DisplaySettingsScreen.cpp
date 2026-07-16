#include "DisplaySettingsScreen.h"

#include <Arduino.h>
#include <lvgl.h>

#include "core/theme/ThemeManager.h"

DisplaySettingsScreen NQDisplaySettings;

const char *DisplaySettingsScreen::screenTitle() const
{
    return "Pantalla";
}

void DisplaySettingsScreen::buildContent(
    lv_obj_t *content
)
{
    lv_obj_set_flex_flow(
        content,
        LV_FLEX_FLOW_COLUMN
    );

    lv_obj_set_flex_align(
        content,
        LV_FLEX_ALIGN_START,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER
    );

    lv_obj_set_style_pad_row(
        content,
        12,
        0
    );

    lv_obj_set_style_pad_bottom(
        content,
        30,
        0
    );

    /*
     * Sección Brillo.
     */
    createSectionTitle(
        content,
        "Brillo"
    );

    lv_obj_t *brightnessCard =
        lv_obj_create(content);

    lv_obj_set_size(
        brightnessCard,
        390,
        112
    );

    lv_obj_set_style_bg_color(
        brightnessCard,
        NQTheme.primaryButton(),
        0
    );

    lv_obj_set_style_bg_opa(
        brightnessCard,
        LV_OPA_COVER,
        0
    );

    lv_obj_set_style_radius(
        brightnessCard,
        20,
        0
    );

    lv_obj_set_style_border_width(
        brightnessCard,
        0,
        0
    );

    lv_obj_set_style_pad_all(
        brightnessCard,
        18,
        0
    );

    lv_obj_clear_flag(
        brightnessCard,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_t *brightnessLabel =
        lv_label_create(brightnessCard);

    lv_label_set_text(
        brightnessLabel,
        "Brillo manual"
    );

    lv_obj_set_style_text_color(
        brightnessLabel,
        NQTheme.text(),
        0
    );

    lv_obj_set_style_text_font(
        brightnessLabel,
        &lv_font_montserrat_18,
        0
    );

    lv_obj_align(
        brightnessLabel,
        LV_ALIGN_TOP_LEFT,
        0,
        0
    );

    brightnessValueLabel =
        lv_label_create(brightnessCard);

    lv_label_set_text(
        brightnessValueLabel,
        "70%"
    );

    lv_obj_set_style_text_color(
        brightnessValueLabel,
        lv_color_hex(0x94A3B8),
        0
    );

    lv_obj_set_style_text_font(
        brightnessValueLabel,
        &lv_font_montserrat_16,
        0
    );

    lv_obj_align(
        brightnessValueLabel,
        LV_ALIGN_TOP_RIGHT,
        0,
        1
    );

    lv_obj_t *brightnessSlider =
        lv_slider_create(brightnessCard);

    lv_obj_set_size(
        brightnessSlider,
        350,
        18
    );

    lv_obj_align(
        brightnessSlider,
        LV_ALIGN_BOTTOM_MID,
        0,
        -8
    );

    lv_slider_set_range(
        brightnessSlider,
        10,
        100
    );

    lv_slider_set_value(
        brightnessSlider,
        70,
        LV_ANIM_OFF
    );

    lv_obj_set_style_bg_color(
        brightnessSlider,
        lv_color_hex(0x334155),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        brightnessSlider,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_color(
        brightnessSlider,
        lv_color_hex(0x22D3EE),
        LV_PART_INDICATOR
    );

    lv_obj_set_style_bg_opa(
        brightnessSlider,
        LV_OPA_COVER,
        LV_PART_INDICATOR
    );

    lv_obj_set_style_bg_color(
        brightnessSlider,
        lv_color_hex(0xF8FAFC),
        LV_PART_KNOB
    );

    lv_obj_set_style_bg_opa(
        brightnessSlider,
        LV_OPA_COVER,
        LV_PART_KNOB
    );

    lv_obj_add_event_cb(
        brightnessSlider,
        brightnessEventHandler,
        LV_EVENT_VALUE_CHANGED,
        this
    );

    /*
     * Opciones de pantalla.
     */
    createSectionTitle(
        content,
        "Comportamiento"
    );

    createOptionCard(
        content,
        "Apagado automatico",
        "30 segundos",
        "screen_timeout"
    );

    createOptionCard(
        content,
        "Tema",
        "Oscuro",
        "theme"
    );

    createOptionCard(
        content,
        "Animaciones",
        "Activadas",
        "animations"
    );

    createOptionCard(
        content,
        "Activar al levantar",
        "Activado",
        "raise_to_wake"
    );
}

lv_obj_t *DisplaySettingsScreen::createSectionTitle(
    lv_obj_t *parent,
    const char *text
)
{
    lv_obj_t *label =
        lv_label_create(parent);

    lv_label_set_text(
        label,
        text
    );

    lv_obj_set_width(
        label,
        390
    );

    lv_obj_set_style_text_color(
        label,
        lv_color_hex(0x94A3B8),
        0
    );

    lv_obj_set_style_text_font(
        label,
        &lv_font_montserrat_16,
        0
    );

    lv_obj_set_style_pad_left(
        label,
        8,
        0
    );

    return label;
}

lv_obj_t *DisplaySettingsScreen::createOptionCard(
    lv_obj_t *parent,
    const char *title,
    const char *value,
    const char *optionId
)
{
    lv_obj_t *card =
        lv_btn_create(parent);

    lv_obj_set_size(
        card,
        390,
        68
    );

    lv_obj_set_style_bg_color(
        card,
        NQTheme.primaryButton(),
        0
    );

    lv_obj_set_style_bg_opa(
        card,
        LV_OPA_COVER,
        0
    );

    lv_obj_set_style_radius(
        card,
        18,
        0
    );

    lv_obj_set_style_border_width(
        card,
        0,
        0
    );

    lv_obj_set_style_shadow_width(
        card,
        0,
        0
    );

    lv_obj_set_style_pad_left(
        card,
        18,
        0
    );

    lv_obj_set_style_pad_right(
        card,
        18,
        0
    );

    lv_obj_clear_flag(
        card,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_add_event_cb(
        card,
        optionEventHandler,
        LV_EVENT_CLICKED,
        const_cast<char *>(optionId)
    );

    lv_obj_t *titleLabel =
        lv_label_create(card);

    lv_label_set_text(
        titleLabel,
        title
    );

    lv_obj_set_style_text_color(
        titleLabel,
        NQTheme.text(),
        0
    );

    lv_obj_set_style_text_font(
        titleLabel,
        &lv_font_montserrat_18,
        0
    );

    lv_obj_align(
        titleLabel,
        LV_ALIGN_LEFT_MID,
        0,
        0
    );

    lv_obj_t *valueLabel =
        lv_label_create(card);

    lv_label_set_text(
        valueLabel,
        value
    );

    lv_obj_set_style_text_color(
        valueLabel,
        lv_color_hex(0x94A3B8),
        0
    );

    lv_obj_set_style_text_font(
        valueLabel,
        &lv_font_montserrat_14,
        0
    );

    lv_obj_align(
        valueLabel,
        LV_ALIGN_RIGHT_MID,
        -20,
        0
    );

    lv_obj_t *arrowLabel =
        lv_label_create(card);

    lv_label_set_text(
        arrowLabel,
        ">"
    );

    lv_obj_set_style_text_color(
        arrowLabel,
        NQTheme.text(),
        0
    );

    lv_obj_set_style_text_font(
        arrowLabel,
        &lv_font_montserrat_18,
        0
    );

    lv_obj_align(
        arrowLabel,
        LV_ALIGN_RIGHT_MID,
        0,
        0
    );

    return card;
}

void DisplaySettingsScreen::brightnessEventHandler(
    lv_event_t *event
)
{
    lv_obj_t *slider =
        lv_event_get_target(event);

    DisplaySettingsScreen *screen =
        static_cast<DisplaySettingsScreen *>(
            lv_event_get_user_data(event)
        );

    if (
        slider == nullptr ||
        screen == nullptr ||
        screen->brightnessValueLabel == nullptr
    )
    {
        return;
    }

    const int value =
        lv_slider_get_value(slider);

    char valueText[8];

    snprintf(
        valueText,
        sizeof(valueText),
        "%d%%",
        value
    );

    lv_label_set_text(
        screen->brightnessValueLabel,
        valueText
    );

    Serial.print(
        "[DisplaySettings] Brillo visual: "
    );

    Serial.println(value);
}

void DisplaySettingsScreen::optionEventHandler(
    lv_event_t *event
)
{
    if (
        lv_event_get_code(event) !=
        LV_EVENT_CLICKED
    )
    {
        return;
    }

    const char *optionId =
        static_cast<const char *>(
            lv_event_get_user_data(event)
        );

    if (optionId == nullptr)
    {
        return;
    }

    Serial.print(
        "[DisplaySettings] Opcion seleccionada: "
    );

    Serial.println(optionId);
}