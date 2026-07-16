#include "SoundSettingsScreen.h"

#include <Arduino.h>
#include <cstdio>
#include <lvgl.h>

namespace
{
    constexpr lv_coord_t CONTENT_WIDTH = 342;
    constexpr lv_coord_t CARD_HEIGHT = 76;

    constexpr uint32_t COLOR_BACKGROUND = 0x020810;
    constexpr uint32_t COLOR_CARD_GRADIENT = 0x071B2A;

    constexpr uint32_t COLOR_CYAN = 0x22D3EE;
    constexpr uint32_t COLOR_CYAN_LIGHT = 0x67E8F9;

    constexpr uint32_t COLOR_PURPLE = 0xA855F7;
    constexpr uint32_t COLOR_PURPLE_LIGHT = 0xC084FC;

    constexpr uint32_t COLOR_TEXT = 0xF8FAFC;
    constexpr uint32_t COLOR_SECONDARY_TEXT = 0x94A3B8;
    constexpr uint32_t COLOR_INACTIVE = 0x334155;

    constexpr int SENSITIVITY_LOW = 0;
    constexpr int SENSITIVITY_MEDIUM = 1;
    constexpr int SENSITIVITY_HIGH = 2;
}

SoundSettingsScreen NQSoundSettings;

const char *SoundSettingsScreen::screenTitle() const
{
    return "Sonido";
}

void SoundSettingsScreen::buildContent(
    lv_obj_t *content
)
{
    /*
     * Esta pantalla necesita desplazamiento vertical porque
     * contiene más controles que los que caben simultáneamente
     * en la zona circular visible.
     */
    lv_obj_add_flag(
        content,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_set_scroll_dir(
        content,
        LV_DIR_VER
    );

    lv_obj_set_scrollbar_mode(
        content,
        LV_SCROLLBAR_MODE_OFF
    );

    lv_obj_clear_flag(
        content,
        LV_OBJ_FLAG_SCROLL_ELASTIC
    );

    lv_obj_add_flag(
        content,
        LV_OBJ_FLAG_SCROLL_MOMENTUM
    );

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

    lv_obj_set_style_pad_top(
        content,
        8,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_bottom(
        content,
        34,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_row(
        content,
        12,
        LV_PART_MAIN
    );

    /*
     * Volumen general.
     */
    createSectionTitle(
        content,
        "Volumen general"
    );

    lv_obj_t *volumeCard =
        lv_obj_create(content);

    lv_obj_set_size(
        volumeCard,
        CONTENT_WIDTH,
        112
    );

    lv_obj_set_style_bg_color(
        volumeCard,
        lv_color_hex(COLOR_BACKGROUND),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_color(
        volumeCard,
        lv_color_hex(COLOR_CARD_GRADIENT),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_dir(
        volumeCard,
        LV_GRAD_DIR_VER,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        volumeCard,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        volumeCard,
        2,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        volumeCard,
        lv_color_hex(COLOR_CYAN),
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        volumeCard,
        26,
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_width(
        volumeCard,
        8,
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_color(
        volumeCard,
        lv_color_hex(COLOR_CYAN),
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_opa(
        volumeCard,
        LV_OPA_20,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        volumeCard,
        18,
        LV_PART_MAIN
    );

    lv_obj_clear_flag(
        volumeCard,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_t *volumeIcon =
        lv_label_create(volumeCard);

    lv_label_set_text(
        volumeIcon,
        LV_SYMBOL_VOLUME_MAX
    );

    lv_obj_set_style_text_color(
        volumeIcon,
        lv_color_hex(COLOR_CYAN_LIGHT),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        volumeIcon,
        &lv_font_montserrat_24,
        LV_PART_MAIN
    );

    lv_obj_align(
        volumeIcon,
        LV_ALIGN_TOP_LEFT,
        0,
        0
    );

    volumeValueLabel =
        lv_label_create(volumeCard);

    lv_label_set_text(
        volumeValueLabel,
        "70%"
    );

    lv_obj_set_style_text_color(
        volumeValueLabel,
        lv_color_hex(COLOR_TEXT),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        volumeValueLabel,
        &lv_font_montserrat_18,
        LV_PART_MAIN
    );

    lv_obj_align(
        volumeValueLabel,
        LV_ALIGN_TOP_RIGHT,
        0,
        1
    );

    lv_obj_t *volumeSlider =
        lv_slider_create(volumeCard);

    lv_obj_set_size(
        volumeSlider,
        294,
        18
    );

    lv_obj_align(
        volumeSlider,
        LV_ALIGN_BOTTOM_MID,
        0,
        -8
    );

    lv_slider_set_range(
        volumeSlider,
        0,
        100
    );

    lv_slider_set_value(
        volumeSlider,
        70,
        LV_ANIM_OFF
    );

    lv_obj_set_style_bg_color(
        volumeSlider,
        lv_color_hex(COLOR_INACTIVE),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_color(
        volumeSlider,
        lv_color_hex(COLOR_CYAN),
        LV_PART_INDICATOR
    );

    lv_obj_set_style_bg_color(
        volumeSlider,
        lv_color_hex(COLOR_TEXT),
        LV_PART_KNOB
    );

    lv_obj_set_style_shadow_width(
        volumeSlider,
        8,
        LV_PART_KNOB
    );

    lv_obj_set_style_shadow_color(
        volumeSlider,
        lv_color_hex(COLOR_CYAN),
        LV_PART_KNOB
    );

    lv_obj_add_event_cb(
        volumeSlider,
        volumeEventHandler,
        LV_EVENT_VALUE_CHANGED,
        this
    );

    /*
     * Opciones generales.
     */
    createSectionTitle(
        content,
        "Audio"
    );

    createSwitchCard(
        content,
        LV_SYMBOL_BELL,
        "Sonidos del sistema",
        "Avisos y confirmaciones",
        "system_sounds",
        true
    );

    createSwitchCard(
        content,
        LV_SYMBOL_AUDIO,
        "Voz de Nauty",
        "Respuestas de Nauty Soul",
        "nauty_voice",
        true
    );

    createSwitchCard(
        content,
        LV_SYMBOL_EYE_OPEN,
        "Microfono",
        "Escucha y comandos locales",
        "microphone",
        true
    );

    /*
     * Sensibilidad.
     */
    createSectionTitle(
        content,
        "Sensibilidad del microfono"
    );

    lv_obj_t *sensitivityCard =
        lv_obj_create(content);

    lv_obj_set_size(
        sensitivityCard,
        CONTENT_WIDTH,
        94
    );

    lv_obj_set_style_bg_color(
        sensitivityCard,
        lv_color_hex(COLOR_BACKGROUND),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_color(
        sensitivityCard,
        lv_color_hex(0x211044),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_dir(
        sensitivityCard,
        LV_GRAD_DIR_VER,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        sensitivityCard,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        sensitivityCard,
        2,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        sensitivityCard,
        lv_color_hex(COLOR_PURPLE),
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        sensitivityCard,
        26,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        sensitivityCard,
        12,
        LV_PART_MAIN
    );

    lv_obj_clear_flag(
        sensitivityCard,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lowSensitivityButton =
        createSensitivityButton(
            sensitivityCard,
            "Baja",
            SENSITIVITY_LOW
        );

    lv_obj_align(
        lowSensitivityButton,
        LV_ALIGN_LEFT_MID,
        0,
        0
    );

    mediumSensitivityButton =
        createSensitivityButton(
            sensitivityCard,
            "Media",
            SENSITIVITY_MEDIUM
        );

    lv_obj_align(
        mediumSensitivityButton,
        LV_ALIGN_CENTER,
        0,
        0
    );

    highSensitivityButton =
        createSensitivityButton(
            sensitivityCard,
            "Alta",
            SENSITIVITY_HIGH
        );

    lv_obj_align(
        highSensitivityButton,
        LV_ALIGN_RIGHT_MID,
        0,
        0
    );

    updateSensitivitySelection(
        SENSITIVITY_MEDIUM
    );

    /*
     * Prueba de altavoz.
     */
    createSectionTitle(
        content,
        "Comprobacion"
    );

    lv_obj_t *speakerTestButton =
        lv_btn_create(content);

    lv_obj_set_size(
        speakerTestButton,
        CONTENT_WIDTH,
        70
    );

    lv_obj_set_style_bg_color(
        speakerTestButton,
        lv_color_hex(COLOR_BACKGROUND),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_color(
        speakerTestButton,
        lv_color_hex(0x052D32),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_dir(
        speakerTestButton,
        LV_GRAD_DIR_HOR,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        speakerTestButton,
        2,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        speakerTestButton,
        lv_color_hex(COLOR_CYAN),
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        speakerTestButton,
        24,
        LV_PART_MAIN
    );

    const lv_style_selector_t pressedSelector =
        static_cast<lv_style_selector_t>(
            LV_PART_MAIN | LV_STATE_PRESSED
        );

    lv_obj_set_style_transform_zoom(
        speakerTestButton,
        244,
        pressedSelector
    );

    lv_obj_clear_flag(
        speakerTestButton,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_add_event_cb(
        speakerTestButton,
        speakerTestEventHandler,
        LV_EVENT_CLICKED,
        nullptr
    );

    lv_obj_t *speakerTestIcon =
        lv_label_create(speakerTestButton);

    lv_label_set_text(
        speakerTestIcon,
        LV_SYMBOL_PLAY
    );

    lv_obj_set_style_text_color(
        speakerTestIcon,
        lv_color_hex(COLOR_CYAN_LIGHT),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        speakerTestIcon,
        &lv_font_montserrat_20,
        LV_PART_MAIN
    );

    lv_obj_align(
        speakerTestIcon,
        LV_ALIGN_LEFT_MID,
        24,
        0
    );

    lv_obj_t *speakerTestLabel =
        lv_label_create(speakerTestButton);

    lv_label_set_text(
        speakerTestLabel,
        "Probar altavoz"
    );

    lv_obj_set_style_text_color(
        speakerTestLabel,
        lv_color_hex(COLOR_TEXT),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        speakerTestLabel,
        &lv_font_montserrat_18,
        LV_PART_MAIN
    );

    lv_obj_align(
        speakerTestLabel,
        LV_ALIGN_CENTER,
        12,
        0
    );
}

lv_obj_t *SoundSettingsScreen::createSectionTitle(
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
        CONTENT_WIDTH
    );

    lv_obj_set_style_text_color(
        label,
        lv_color_hex(COLOR_SECONDARY_TEXT),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        label,
        &lv_font_montserrat_14,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_left(
        label,
        14,
        LV_PART_MAIN
    );

    return label;
}

lv_obj_t *SoundSettingsScreen::createSwitchCard(
    lv_obj_t *parent,
    const char *icon,
    const char *title,
    const char *description,
    const char *settingId,
    bool enabled
)
{
    lv_obj_t *card =
        lv_obj_create(parent);

    lv_obj_set_size(
        card,
        CONTENT_WIDTH,
        CARD_HEIGHT
    );

    lv_obj_set_style_bg_color(
        card,
        lv_color_hex(COLOR_BACKGROUND),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_color(
        card,
        lv_color_hex(COLOR_CARD_GRADIENT),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_dir(
        card,
        LV_GRAD_DIR_HOR,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        card,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        card,
        1,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        card,
        lv_color_hex(COLOR_CYAN),
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        card,
        24,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        card,
        14,
        LV_PART_MAIN
    );

    lv_obj_clear_flag(
        card,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_t *iconLabel =
        lv_label_create(card);

    lv_label_set_text(
        iconLabel,
        icon
    );

    lv_obj_set_style_text_color(
        iconLabel,
        lv_color_hex(COLOR_CYAN_LIGHT),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        iconLabel,
        &lv_font_montserrat_20,
        LV_PART_MAIN
    );

    lv_obj_align(
        iconLabel,
        LV_ALIGN_LEFT_MID,
        2,
        0
    );

    lv_obj_t *titleLabel =
        lv_label_create(card);

    lv_label_set_text(
        titleLabel,
        title
    );

    lv_obj_set_style_text_color(
        titleLabel,
        lv_color_hex(COLOR_TEXT),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        titleLabel,
        &lv_font_montserrat_18,
        LV_PART_MAIN
    );

    lv_obj_align(
        titleLabel,
        LV_ALIGN_LEFT_MID,
        38,
        -12
    );

    lv_obj_t *descriptionLabel =
        lv_label_create(card);

    lv_label_set_text(
        descriptionLabel,
        description
    );

    lv_obj_set_style_text_color(
        descriptionLabel,
        lv_color_hex(COLOR_SECONDARY_TEXT),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        descriptionLabel,
        &lv_font_montserrat_14,
        LV_PART_MAIN
    );

    lv_obj_align(
        descriptionLabel,
        LV_ALIGN_LEFT_MID,
        38,
        14
    );

    lv_obj_t *settingSwitch =
        lv_switch_create(card);

    lv_obj_set_size(
        settingSwitch,
        52,
        28
    );

    lv_obj_align(
        settingSwitch,
        LV_ALIGN_RIGHT_MID,
        0,
        0
    );

    lv_obj_set_style_bg_color(
        settingSwitch,
        lv_color_hex(COLOR_INACTIVE),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_color(
        settingSwitch,
        lv_color_hex(COLOR_CYAN),
        LV_PART_INDICATOR | LV_STATE_CHECKED
    );

    lv_obj_set_style_bg_color(
        settingSwitch,
        lv_color_hex(COLOR_TEXT),
        LV_PART_KNOB
    );

    if (enabled)
    {
        lv_obj_add_state(
            settingSwitch,
            LV_STATE_CHECKED
        );
    }

    lv_obj_add_event_cb(
        settingSwitch,
        switchEventHandler,
        LV_EVENT_VALUE_CHANGED,
        const_cast<char *>(settingId)
    );

    return card;
}

lv_obj_t *SoundSettingsScreen::createSensitivityButton(
    lv_obj_t *parent,
    const char *text,
    int value
)
{
    lv_obj_t *button =
        lv_btn_create(parent);

    lv_obj_set_size(
        button,
        94,
        54
    );

    lv_obj_set_style_bg_color(
        button,
        lv_color_hex(COLOR_BACKGROUND),
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        button,
        1,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        button,
        lv_color_hex(COLOR_PURPLE),
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        button,
        18,
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_width(
        button,
        0,
        LV_PART_MAIN
    );

    lv_obj_add_event_cb(
        button,
        sensitivityEventHandler,
        LV_EVENT_CLICKED,
        reinterpret_cast<void *>(
            static_cast<intptr_t>(value)
        )
    );

    lv_obj_t *label =
        lv_label_create(button);

    lv_label_set_text(
        label,
        text
    );

    lv_obj_set_style_text_color(
        label,
        lv_color_hex(COLOR_TEXT),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        label,
        &lv_font_montserrat_14,
        LV_PART_MAIN
    );

    lv_obj_center(label);

    return button;
}

void SoundSettingsScreen::updateSensitivitySelection(
    int value
)
{
    selectedSensitivity = value;

    lv_obj_t *buttons[] =
    {
        lowSensitivityButton,
        mediumSensitivityButton,
        highSensitivityButton
    };

    for (int index = 0; index < 3; index++)
    {
        lv_obj_t *button = buttons[index];

        if (button == nullptr)
        {
            continue;
        }

        const bool selected =
            index == selectedSensitivity;

        lv_obj_set_style_bg_color(
            button,
            lv_color_hex(
                selected
                    ? 0x3B1760
                    : COLOR_BACKGROUND
            ),
            LV_PART_MAIN
        );

        lv_obj_set_style_border_color(
            button,
            lv_color_hex(
                selected
                    ? COLOR_PURPLE_LIGHT
                    : COLOR_PURPLE
            ),
            LV_PART_MAIN
        );

        lv_obj_set_style_shadow_width(
            button,
            selected ? 8 : 0,
            LV_PART_MAIN
        );

        lv_obj_set_style_shadow_color(
            button,
            lv_color_hex(COLOR_PURPLE),
            LV_PART_MAIN
        );

        lv_obj_set_style_shadow_opa(
            button,
            selected ? LV_OPA_20 : LV_OPA_0,
            LV_PART_MAIN
        );
    }
}

void SoundSettingsScreen::volumeEventHandler(
    lv_event_t *event
)
{
    lv_obj_t *slider =
        lv_event_get_target(event);

    SoundSettingsScreen *screen =
        static_cast<SoundSettingsScreen *>(
            lv_event_get_user_data(event)
        );

    if (
        slider == nullptr ||
        screen == nullptr ||
        screen->volumeValueLabel == nullptr
    )
    {
        return;
    }

    const int value =
        lv_slider_get_value(slider);

    char valueText[8];

    std::snprintf(
        valueText,
        sizeof(valueText),
        "%d%%",
        value
    );

    lv_label_set_text(
        screen->volumeValueLabel,
        valueText
    );

    Serial.print(
        "[SoundSettings] Volumen visual: "
    );

    Serial.println(value);
}

void SoundSettingsScreen::switchEventHandler(
    lv_event_t *event
)
{
    lv_obj_t *settingSwitch =
        lv_event_get_target(event);

    const char *settingId =
        static_cast<const char *>(
            lv_event_get_user_data(event)
        );

    if (
        settingSwitch == nullptr ||
        settingId == nullptr
    )
    {
        return;
    }

    const bool enabled =
        lv_obj_has_state(
            settingSwitch,
            LV_STATE_CHECKED
        );

    Serial.print("[SoundSettings] ");
    Serial.print(settingId);
    Serial.print(": ");
    Serial.println(
        enabled ? "ON" : "OFF"
    );
}

void SoundSettingsScreen::sensitivityEventHandler(
    lv_event_t *event
)
{
    SoundSettingsScreen *screen =
        &NQSoundSettings;

    const int value =
        static_cast<int>(
            reinterpret_cast<intptr_t>(
                lv_event_get_user_data(event)
            )
        );

    screen->updateSensitivitySelection(value);

    Serial.print(
        "[SoundSettings] Sensibilidad: "
    );

    switch (value)
    {
        case SENSITIVITY_LOW:
            Serial.println("Baja");
            break;

        case SENSITIVITY_HIGH:
            Serial.println("Alta");
            break;

        case SENSITIVITY_MEDIUM:
        default:
            Serial.println("Media");
            break;
    }
}

void SoundSettingsScreen::speakerTestEventHandler(
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

    Serial.println(
        "[SoundSettings] Prueba de altavoz solicitada"
    );

    /*
     * En la fase de hardware conectaremos aquí
     * el codec ES8311 y el amplificador.
     */
}