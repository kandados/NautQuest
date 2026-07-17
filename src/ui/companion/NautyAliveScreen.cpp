#include "NautyAliveScreen.h"

#include <Arduino.h>
#include <lvgl.h>

#include "core/NavigationManager.h"
#include "storage/AssetManager.h"
#include "ui/companion/NautyAnimator.h"

NautyAliveScreen NQNautyAlive;

namespace
{
    /*
     * Selectores de estilo utilizados por el botón.
     *
     * El cast evita los warnings producidos al combinar
     * LV_PART_MAIN con los estados de LVGL.
     */
    constexpr lv_style_selector_t STYLE_DEFAULT =
        static_cast<lv_style_selector_t>(
            LV_PART_MAIN | LV_STATE_DEFAULT
        );

    constexpr lv_style_selector_t STYLE_PRESSED =
        static_cast<lv_style_selector_t>(
            LV_PART_MAIN | LV_STATE_PRESSED
        );

    /*
     * Crea el botón visible para volver.
     */
    lv_obj_t *createBackButton(
        lv_obj_t *parent,
        lv_event_cb_t callback
    )
    {
        lv_obj_t *button = lv_btn_create(parent);

        /*
         * El botón se introduce hacia el centro porque
         * la pantalla es circular.
         */
        lv_obj_set_size(
            button,
            64,
            64
        );

        lv_obj_align(
            button,
            LV_ALIGN_TOP_LEFT,
            60,
            60
        );

        /*
         * Forma circular.
         */
        lv_obj_set_style_radius(
            button,
            LV_RADIUS_CIRCLE,
            LV_PART_MAIN
        );

        /*
         * Estado normal.
         */
        lv_obj_set_style_bg_color(
            button,
            lv_color_hex(0x202020),
            STYLE_DEFAULT
        );

        lv_obj_set_style_bg_opa(
            button,
            LV_OPA_80,
            STYLE_DEFAULT
        );

        /*
         * Estado pulsado.
         */
        lv_obj_set_style_bg_color(
            button,
            lv_color_hex(0x404040),
            STYLE_PRESSED
        );

        lv_obj_set_style_bg_opa(
            button,
            LV_OPA_COVER,
            STYLE_PRESSED
        );

        /*
         * Borde discreto.
         */
        lv_obj_set_style_border_width(
            button,
            1,
            LV_PART_MAIN
        );

        lv_obj_set_style_border_color(
            button,
            lv_color_hex(0x707070),
            LV_PART_MAIN
        );

        lv_obj_set_style_shadow_width(
            button,
            0,
            LV_PART_MAIN
        );

        lv_obj_set_style_outline_width(
            button,
            0,
            LV_PART_MAIN
        );

        lv_obj_clear_flag(
            button,
            LV_OBJ_FLAG_SCROLLABLE
        );

        lv_obj_add_event_cb(
            button,
            callback,
            LV_EVENT_CLICKED,
            nullptr
        );

        /*
         * Flecha de retorno.
         */
        lv_obj_t *icon = lv_label_create(button);

        lv_label_set_text(
            icon,
            LV_SYMBOL_LEFT
        );

        lv_obj_set_style_text_color(
            icon,
            lv_color_white(),
            0
        );

        lv_obj_set_style_text_font(
            icon,
            &lv_font_montserrat_18,
            0
        );

        lv_obj_center(icon);

        /*
         * Garantiza que el botón quede por encima
         * del casco y de los elementos del animador.
         */
        lv_obj_move_foreground(button);

        return button;
    }
}

void NautyAliveScreen::backEvent(
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
        "[NautyAliveScreen] Volviendo a la portada de Nauty"
    );

    NQNavigation.goBack();
}

void NautyAliveScreen::show()
{
    /*
     * Eliminamos primero todos los objetos pertenecientes
     * a la pantalla anterior.
     */
    lv_obj_clean(
        lv_scr_act()
    );

    /*
     * Una vez eliminados los objetos LVGL, ya es seguro
     * liberar las imágenes almacenadas en la caché.
     */
    NQAssets.clearImageCache();

    /*
     * Fondo negro.
     */
    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_black(),
        0
    );

    lv_obj_set_style_bg_opa(
        lv_scr_act(),
        LV_OPA_COVER,
        0
    );

    lv_obj_clear_flag(
        lv_scr_act(),
        LV_OBJ_FLAG_SCROLLABLE
    );

    /*
     * NautyAnimator se encarga de cargar y mostrar
     * el casco y, posteriormente, sus animaciones.
     */
    NQNautyAnimator.begin(
        lv_scr_act()
    );

    /*
     * El botón se crea después del animador para que
     * aparezca siempre por encima del casco.
     */
    createBackButton(
        lv_scr_act(),
        backEvent
    );

    /*
     * Fuerza una actualización inmediata de LVGL para
     * mostrar la pantalla sin esperar al siguiente ciclo.
     */
    lv_timer_handler();

    Serial.println(
        "[NautyAliveScreen] NautyAnimator iniciado"
    );
}