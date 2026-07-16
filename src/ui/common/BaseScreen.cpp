#include "BaseScreen.h"

#include <Arduino.h>

#include "core/NavigationManager.h"
#include "core/ScreenManager.h"

namespace
{
    constexpr lv_coord_t SCREEN_WIDTH = 466;
    constexpr lv_coord_t SCREEN_HEIGHT = 466;

    /*
     * Cabecera más alta para colocar:
     *
     * 1. Botón de volver centrado.
     * 2. Título debajo.
     */
    constexpr lv_coord_t HEADER_HEIGHT = 120;
    constexpr lv_coord_t BOTTOM_BAR_HEIGHT = 46;

    constexpr lv_coord_t CONTENT_SIDE_MARGIN = 0;
    constexpr lv_coord_t CONTENT_TOP_MARGIN = 0;
    constexpr lv_coord_t CONTENT_BOTTOM_MARGIN = 0;

    /*
     * Botón de volver centrado horizontalmente.
     *
     * X = (466 - 54) / 2 = 206
     */
    constexpr lv_coord_t BACK_BUTTON_SIZE = 54;
    constexpr lv_coord_t BACK_BUTTON_X =
        (SCREEN_WIDTH - BACK_BUTTON_SIZE) / 2;

    constexpr lv_coord_t BACK_BUTTON_Y = 8;

    /*
     * Posición vertical del título debajo del botón.
     */
    constexpr lv_coord_t TITLE_Y = 75;
}

void BaseScreen::show()
{
    Serial.print("[BaseScreen] Mostrando: ");
    Serial.println(screenTitle());

    createRoot();
    createHeader();
    createContent();

    if (hasBottomBar())
    {
        createBottomBar();
    }

    buildContent(content);
    onScreenReady();

    if (hasBackButton())
    {
        createBackTouchOverlay();
    }

    lv_timer_handler();
}

bool BaseScreen::hasBackButton() const
{
    return true;
}

bool BaseScreen::hasBottomBar() const
{
    return false;
}

void BaseScreen::onScreenReady()
{
}

lv_obj_t *BaseScreen::rootObject() const
{
    return root;
}

lv_obj_t *BaseScreen::headerObject() const
{
    return header;
}

lv_obj_t *BaseScreen::contentObject() const
{
    return content;
}

lv_obj_t *BaseScreen::bottomBarObject() const
{
    return bottomBar;
}

void BaseScreen::createRoot()
{
    /*
     * Elimina todos los objetos de la pantalla anterior.
     */
    lv_obj_clean(lv_scr_act());

    root = lv_obj_create(lv_scr_act());

    lv_obj_set_size(
        root,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );

    lv_obj_center(root);

    lv_obj_set_style_bg_color(
        root,
        lv_color_black(),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        root,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        root,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        root,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        root,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_scrollbar_mode(
        root,
        LV_SCROLLBAR_MODE_OFF
    );

    lv_obj_clear_flag(
        root,
        LV_OBJ_FLAG_SCROLLABLE
    );
}

void BaseScreen::createHeader()
{
    header = lv_obj_create(root);

    lv_obj_set_size(
        header,
        SCREEN_WIDTH,
        HEADER_HEIGHT
    );

    lv_obj_align(
        header,
        LV_ALIGN_TOP_MID,
        0,
        0
    );

    lv_obj_set_style_bg_color(
        header,
        lv_color_black(),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        header,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        header,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        header,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        header,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_scrollbar_mode(
        header,
        LV_SCROLLBAR_MODE_OFF
    );

    lv_obj_clear_flag(
        header,
        LV_OBJ_FLAG_SCROLLABLE
    );

    /*
     * Título centrado horizontalmente y colocado
     * debajo del botón de volver.
     */
    lv_obj_t *titleLabel =
        lv_label_create(header);

    lv_label_set_text(
        titleLabel,
        screenTitle()
    );

    lv_obj_set_style_text_color(
        titleLabel,
        lv_color_hex(0xF8FAFC),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        titleLabel,
        &lv_font_montserrat_24,
        LV_PART_MAIN
    );

    lv_obj_align(
        titleLabel,
        LV_ALIGN_TOP_MID,
        0,
        TITLE_Y
    );
}

void BaseScreen::createContent()
{
    lv_coord_t contentHeight =
        SCREEN_HEIGHT -
        HEADER_HEIGHT -
        CONTENT_TOP_MARGIN -
        CONTENT_BOTTOM_MARGIN;

    if (hasBottomBar())
    {
        contentHeight -= BOTTOM_BAR_HEIGHT;
    }

    content = lv_obj_create(root);

    lv_obj_set_size(
        content,
        SCREEN_WIDTH - (CONTENT_SIDE_MARGIN * 2),
        contentHeight
    );

    lv_obj_align(
        content,
        LV_ALIGN_TOP_MID,
        0,
        HEADER_HEIGHT + CONTENT_TOP_MARGIN
    );

    lv_obj_set_style_bg_opa(
        content,
        LV_OPA_0,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        content,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        content,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        content,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_scrollbar_mode(
        content,
        LV_SCROLLBAR_MODE_OFF
    );

    /*
     * Cada pantalla decide si necesita desplazamiento.
     */
    lv_obj_clear_flag(
        content,
        LV_OBJ_FLAG_SCROLLABLE
    );
}

void BaseScreen::createBottomBar()
{
    bottomBar = lv_obj_create(root);

    lv_obj_set_size(
        bottomBar,
        SCREEN_WIDTH,
        BOTTOM_BAR_HEIGHT
    );

    lv_obj_align(
        bottomBar,
        LV_ALIGN_BOTTOM_MID,
        0,
        0
    );

    lv_obj_set_style_bg_color(
        bottomBar,
        lv_color_black(),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        bottomBar,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        bottomBar,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        bottomBar,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        bottomBar,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_scrollbar_mode(
        bottomBar,
        LV_SCROLLBAR_MODE_OFF
    );

    lv_obj_clear_flag(
        bottomBar,
        LV_OBJ_FLAG_SCROLLABLE
    );
}

void BaseScreen::createBackTouchOverlay()
{
    /*
     * Se crea directamente sobre la pantalla activa para
     * garantizar que siempre quede por encima del contenido.
     */
    lv_obj_t *backButton =
        lv_btn_create(lv_scr_act());

    lv_obj_set_size(
        backButton,
        BACK_BUTTON_SIZE,
        BACK_BUTTON_SIZE
    );

    /*
     * Botón centrado horizontalmente.
     */
    lv_obj_set_pos(
        backButton,
        BACK_BUTTON_X,
        BACK_BUTTON_Y
    );

    /*
     * Fondo oscuro con degradado sutil.
     */
    lv_obj_set_style_bg_color(
        backButton,
        lv_color_hex(0x020810),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_color(
        backButton,
        lv_color_hex(0x06243A),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_grad_dir(
        backButton,
        LV_GRAD_DIR_VER,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        backButton,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    /*
     * Contorno cian.
     */
    lv_obj_set_style_border_width(
        backButton,
        2,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        backButton,
        lv_color_hex(0x00D8FF),
        LV_PART_MAIN
    );

    lv_obj_set_style_border_opa(
        backButton,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        backButton,
        LV_RADIUS_CIRCLE,
        LV_PART_MAIN
    );

    /*
     * Resplandor moderado.
     */
    lv_obj_set_style_shadow_width(
        backButton,
        9,
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_spread(
        backButton,
        1,
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_color(
        backButton,
        lv_color_hex(0x00BFFF),
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_opa(
        backButton,
        LV_OPA_20,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        backButton,
        0,
        LV_PART_MAIN
    );

    /*
     * Respuesta visual al pulsar.
     */
    const lv_style_selector_t pressedSelector =
        static_cast<lv_style_selector_t>(
            LV_PART_MAIN | LV_STATE_PRESSED
        );

    lv_obj_set_style_bg_color(
        backButton,
        lv_color_hex(0x0B3040),
        pressedSelector
    );

    lv_obj_set_style_transform_zoom(
        backButton,
        240,
        pressedSelector
    );

    /*
     * Área táctil ampliada sin aumentar el botón visible.
     */
    lv_obj_set_ext_click_area(
        backButton,
        16
    );

    lv_obj_clear_flag(
        backButton,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_add_event_cb(
        backButton,
        backButtonEventHandler,
        LV_EVENT_CLICKED,
        nullptr
    );

    /*
     * Flecha central.
     */
    lv_obj_t *backLabel =
        lv_label_create(backButton);

    lv_label_set_text(
        backLabel,
        LV_SYMBOL_LEFT
    );

    lv_obj_set_style_text_color(
        backLabel,
        lv_color_hex(0x67E8F9),
        LV_PART_MAIN
    );

    lv_obj_set_style_text_font(
        backLabel,
        &lv_font_montserrat_24,
        LV_PART_MAIN
    );

    lv_obj_center(backLabel);

    /*
     * La etiqueta no intercepta pulsaciones.
     */
    lv_obj_clear_flag(
        backLabel,
        LV_OBJ_FLAG_CLICKABLE
    );

    lv_obj_move_foreground(backButton);
}

void BaseScreen::backButtonEventHandler(
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
        "[BaseScreen] Volver pulsado"
    );

    if (NQNavigation.canGoBack())
    {
        NQNavigation.goBack();
        return;
    }

    Serial.println(
        "[BaseScreen] Sin historial: regreso a Inicio"
    );

    NQScreen.show(
        ScreenID::Desktop
    );
}