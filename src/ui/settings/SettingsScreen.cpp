#include "SettingsScreen.h"

#include <Arduino.h>
#include <cstring>
#include <lvgl.h>

#include "core/NavigationManager.h"
#include "core/ScreenManager.h"

namespace
{
    /*
     * Dimensiones calculadas para que las nueve tarjetas
     * entren completamente en la zona visible circular.
     */
    constexpr lv_coord_t BUTTON_WIDTH = 92;
    constexpr lv_coord_t BUTTON_HEIGHT = 92;

    constexpr lv_coord_t LEFT_X = -104;
    constexpr lv_coord_t CENTER_X = 0;
    constexpr lv_coord_t RIGHT_X = 104;

    constexpr lv_coord_t TOP_Y = -121;
    constexpr lv_coord_t MIDDLE_Y = -18;
    constexpr lv_coord_t BOTTOM_Y = 85;

    /*
     * Paletas visuales inspiradas en las cubiertas NautQuest.
     */
    constexpr SettingsScreen::ButtonPalette CYAN_PALETTE =
    {
        0x00D8FF,
        0x00BFFF,
        0x06243A,
        0x35E6FF
    };

    constexpr SettingsScreen::ButtonPalette TEAL_PALETTE =
    {
        0x22E6E6,
        0x14B8A6,
        0x052D32,
        0x5FF7F2
    };

    constexpr SettingsScreen::ButtonPalette PURPLE_PALETTE =
    {
        0xA855F7,
        0x7C3AED,
        0x211044,
        0xC084FC
    };
}

SettingsScreen NQSettings;

const char *SettingsScreen::screenTitle() const
{
    return "";
}

void SettingsScreen::buildContent(
    lv_obj_t *content
)
{
    /*
     * Cuadrícula fija.
     *
     * No hay scroll, inercia ni animación autónoma.
     */
    lv_obj_clear_flag(
        content,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_set_scrollbar_mode(
        content,
        LV_SCROLLBAR_MODE_OFF
    );

    /*
     * Primera fila.
     */
    createSettingsButton(
        content,
        LV_SYMBOL_IMAGE,
        "Pantalla",
        "display",
        LEFT_X,
        TOP_Y,
        CYAN_PALETTE
    );

    createSettingsButton(
        content,
        LV_SYMBOL_AUDIO,
        "Sonido",
        "sound",
        CENTER_X,
        TOP_Y,
        TEAL_PALETTE
    );

    createSettingsButton(
        content,
        LV_SYMBOL_WIFI,
        "Red",
        "connectivity",
        RIGHT_X,
        TOP_Y,
        PURPLE_PALETTE
    );

    /*
     * Segunda fila.
     */
    createSettingsButton(
        content,
        LV_SYMBOL_BATTERY_FULL,
        "Energia",
        "power",
        LEFT_X,
        MIDDLE_Y,
        CYAN_PALETTE
    );

    createSettingsButton(
        content,
        LV_SYMBOL_SETTINGS,
        "Sistema",
        "system",
        CENTER_X,
        MIDDLE_Y,
        TEAL_PALETTE
    );

    createSettingsButton(
        content,
        LV_SYMBOL_DRIVE,
        "Hardware",
        "device",
        RIGHT_X,
        MIDDLE_Y,
        PURPLE_PALETTE
    );

    /*
     * Tercera fila.
     */
    createSettingsButton(
        content,
        "i",
        "Info",
        "about",
        LEFT_X,
        BOTTOM_Y,
        CYAN_PALETTE
    );

    createSettingsButton(
        content,
        LV_SYMBOL_HOME,
        "Inicio",
        "home",
        CENTER_X,
        BOTTOM_Y,
        TEAL_PALETTE
    );

    createSettingsButton(
        content,
        LV_SYMBOL_LIST,
        "Mas",
        "more",
        RIGHT_X,
        BOTTOM_Y,
        PURPLE_PALETTE
    );

    createPageIndicators(content);
}

lv_obj_t *SettingsScreen::createSettingsButton(
    lv_obj_t *parent,
    const char *icon,
    const char *title,
    const char *itemId,
    lv_coord_t x,
    lv_coord_t y,
    const ButtonPalette &palette
)
{
    lv_obj_t *button =
        lv_btn_create(parent);

    lv_obj_set_size(
        button,
        BUTTON_WIDTH,
        BUTTON_HEIGHT
    );

    lv_obj_align(
        button,
        LV_ALIGN_CENTER,
        x,
        y
    );

    /*
     * Fondo oscuro tipo cristal.
     */
    lv_obj_set_style_bg_color(
        button,
        lv_color_hex(0x020810),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_color(
        button,
        lv_color_hex(palette.gradient),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_dir(
        button,
        LV_GRAD_DIR_VER,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        button,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    /*
     * Contorno luminoso.
     */
    lv_obj_set_style_border_width(
        button,
        2,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        button,
        lv_color_hex(palette.border),
        LV_PART_MAIN
    );

    lv_obj_set_style_border_opa(
        button,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        button,
        28,
        LV_PART_MAIN
    );

    /*
     * Resplandor exterior moderado.
     */
    lv_obj_set_style_shadow_width(
        button,
        9,
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_spread(
        button,
        1,
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_color(
        button,
        lv_color_hex(palette.glow),
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_opa(
        button,
        LV_OPA_20,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        button,
        0,
        LV_PART_MAIN
    );

    /*
     * Estado pulsado.
     *
     * Se reduce ligeramente para dar respuesta táctil,
     * sin desplazarse ni desaparecer.
     */
    lv_obj_set_style_bg_color(
        button,
        lv_color_hex(palette.gradient),
        static_cast<lv_style_selector_t>(
            LV_PART_MAIN | LV_STATE_PRESSED
        )
    );

    lv_obj_set_style_transform_zoom(
        button,
        242,
        static_cast<lv_style_selector_t>(
            LV_PART_MAIN | LV_STATE_PRESSED
        )
    );

    lv_obj_set_style_border_color(
        button,
        lv_color_hex(palette.icon),
        static_cast<lv_style_selector_t>(
            LV_PART_MAIN | LV_STATE_PRESSED
        )
    );

    lv_obj_clear_flag(
        button,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_add_event_cb(
        button,
        settingsButtonEventHandler,
        LV_EVENT_CLICKED,
        const_cast<char *>(itemId)
    );

    /*
     * Icono principal.
     */
    lv_obj_t *iconLabel =
        lv_label_create(button);

    lv_label_set_text(
        iconLabel,
        icon
    );

    lv_obj_set_style_text_color(
        iconLabel,
        lv_color_hex(palette.icon),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        iconLabel,
        &lv_font_montserrat_24,
        LV_PART_MAIN
    );

    lv_obj_align(
        iconLabel,
        LV_ALIGN_CENTER,
        0,
        -18
    );

    lv_obj_clear_flag(
        iconLabel,
        LV_OBJ_FLAG_CLICKABLE
    );

    /*
     * Nombre de la categoría.
     */
    lv_obj_t *titleLabel =
        lv_label_create(button);

    lv_label_set_text(
        titleLabel,
        title
    );

    lv_obj_set_width(
        titleLabel,
        BUTTON_WIDTH - 8
    );

    lv_obj_set_style_text_align(
        titleLabel,
        LV_TEXT_ALIGN_CENTER,
        LV_PART_MAIN
    );

    lv_obj_set_style_text_color(
        titleLabel,
        lv_color_hex(0xF8FAFC),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        titleLabel,
        &lv_font_montserrat_14,
        LV_PART_MAIN
    );

    lv_obj_align(
        titleLabel,
        LV_ALIGN_CENTER,
        0,
        18
    );

    lv_obj_clear_flag(
        titleLabel,
        LV_OBJ_FLAG_CLICKABLE
    );

    /*
     * Línea luminosa decorativa inferior.
     */
    lv_obj_t *accentLine =
        lv_obj_create(button);

    lv_obj_set_size(
        accentLine,
        34,
        2
    );

    lv_obj_align(
        accentLine,
        LV_ALIGN_BOTTOM_MID,
        0,
        -9
    );

    lv_obj_set_style_bg_color(
        accentLine,
        lv_color_hex(palette.border),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        accentLine,
        LV_OPA_50,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        accentLine,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        accentLine,
        LV_RADIUS_CIRCLE,
        LV_PART_MAIN
    );

    lv_obj_clear_flag(
        accentLine,
        LV_OBJ_FLAG_CLICKABLE
    );

    lv_obj_clear_flag(
        accentLine,
        LV_OBJ_FLAG_SCROLLABLE
    );

    /*
     * Punto luminoso inferior.
     */
    lv_obj_t *accentDot =
        lv_obj_create(button);

    lv_obj_set_size(
        accentDot,
        6,
        6
    );

    lv_obj_align(
        accentDot,
        LV_ALIGN_BOTTOM_MID,
        0,
        -7
    );

    lv_obj_set_style_bg_color(
        accentDot,
        lv_color_hex(palette.icon),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        accentDot,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        accentDot,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        accentDot,
        LV_RADIUS_CIRCLE,
        LV_PART_MAIN
    );

    lv_obj_clear_flag(
        accentDot,
        LV_OBJ_FLAG_CLICKABLE
    );

    lv_obj_clear_flag(
        accentDot,
        LV_OBJ_FLAG_SCROLLABLE
    );

    return button;
}

void SettingsScreen::createPageIndicators(
    lv_obj_t *parent
)
{
    lv_obj_t *activePage =
        lv_obj_create(parent);

    lv_obj_set_size(
        activePage,
        10,
        10
    );

    lv_obj_align(
        activePage,
        LV_ALIGN_BOTTOM_MID,
        -9,
        -1
    );

    lv_obj_set_style_radius(
        activePage,
        LV_RADIUS_CIRCLE,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_color(
        activePage,
        lv_color_hex(0x22D3EE),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        activePage,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        activePage,
        0,
        LV_PART_MAIN
    );

    lv_obj_clear_flag(
        activePage,
        LV_OBJ_FLAG_CLICKABLE
    );

    lv_obj_clear_flag(
        activePage,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_t *inactivePage =
        lv_obj_create(parent);

    lv_obj_set_size(
        inactivePage,
        10,
        10
    );

    lv_obj_align(
        inactivePage,
        LV_ALIGN_BOTTOM_MID,
        9,
        -1
    );

    lv_obj_set_style_radius(
        inactivePage,
        LV_RADIUS_CIRCLE,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_color(
        inactivePage,
        lv_color_hex(0x475569),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        inactivePage,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        inactivePage,
        0,
        LV_PART_MAIN
    );

    lv_obj_clear_flag(
        inactivePage,
        LV_OBJ_FLAG_CLICKABLE
    );

    lv_obj_clear_flag(
        inactivePage,
        LV_OBJ_FLAG_SCROLLABLE
    );
}

void SettingsScreen::settingsButtonEventHandler(
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

    const char *itemId =
        static_cast<const char *>(
            lv_event_get_user_data(event)
        );

    if (itemId == nullptr)
    {
        return;
    }

    Serial.print(
        "[SettingsScreen] Seleccionado: "
    );

    Serial.println(itemId);

    if (
        std::strcmp(itemId, "display") == 0
    )
    {
        NQNavigation.goTo(
            ScreenID::SettingsDisplay
        );

        return;
    }

    if (
        std::strcmp(itemId, "sound") == 0
    )
    {
        NQNavigation.goTo(
            ScreenID::SettingsSound
        );

        return;
    }

    if (
        std::strcmp(itemId, "home") == 0
    )
    {
        NQScreen.show(
            ScreenID::Desktop
        );

        return;
    }

    Serial.println(
        "[SettingsScreen] Subpantalla pendiente"
    );
}