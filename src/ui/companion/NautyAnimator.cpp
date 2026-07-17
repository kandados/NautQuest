#include "NautyAnimator.h"

#include <Arduino.h>

#include "storage/AssetManager.h"

NautyAnimator NQNautyAnimator;

namespace
{
    constexpr const char *HELMET_PATH =
        "S:/ui/nauty_alive/helmet_base.bin";
}

void NautyAnimator::begin(
    lv_obj_t *parent
)
{
    parent_ = parent;
    helmet_ = nullptr;

    if (parent_ == nullptr)
    {
        Serial.println(
            "[NautyAnimator] ERROR: parent nulo"
        );

        return;
    }

    /*
     * El casco se carga como fondo.
     */
    const lv_img_dsc_t *helmetSource =
        NQAssets.loadImageToPSRAM(
            HELMET_PATH
        );

    if (helmetSource != nullptr)
    {
        helmet_ = lv_img_create(parent_);

        lv_img_set_src(
            helmet_,
            helmetSource
        );

        lv_obj_center(
            helmet_
        );

        lv_obj_clear_flag(
            helmet_,
            LV_OBJ_FLAG_CLICKABLE
        );

        lv_obj_clear_flag(
            helmet_,
            LV_OBJ_FLAG_SCROLLABLE
        );

        Serial.println(
            "[NautyAnimator] Casco cargado"
        );
    }
    else
    {
        Serial.println(
            "[NautyAnimator] helmet_base.bin no encontrado"
        );

        /*
         * El aviso queda abajo para no tapar la cara.
         */
        lv_obj_t *label =
            lv_label_create(parent_);

        lv_label_set_text(
            label,
            "Falta helmet_base.bin"
        );

        lv_obj_set_style_text_color(
            label,
            lv_color_hex(0x909090),
            0
        );

        lv_obj_set_style_text_font(
            label,
            &lv_font_montserrat_18,
            0
        );

        lv_obj_set_style_text_align(
            label,
            LV_TEXT_ALIGN_CENTER,
            0
        );

        lv_obj_align(
            label,
            LV_ALIGN_BOTTOM_MID,
            0,
            -34
        );
    }

    /*
     * La cara se crea después del casco para quedar
     * siempre por encima de la imagen base.
     */
    face_.begin(parent_);

    Serial.println(
        "[NautyAnimator] Cara modular iniciada"
    );
}

void NautyAnimator::update()
{
    face_.update();
}

NautyFace &NautyAnimator::face()
{
    return face_;
}