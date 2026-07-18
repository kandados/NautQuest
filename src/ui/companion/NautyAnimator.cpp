#include "NautyAnimator.h"

#include <Arduino.h>

#include "storage/AssetManager.h"

NautyAnimator NQNautyAnimator;

extern NautyBehavior nautyBehavior;

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
    stateApplied_ = false;
    active_ = false;

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
    active_ = face_.isReady();

    Serial.println(
        "[NautyAnimator] Cara modular iniciada"
    );
}

void NautyAnimator::stop()
{
    active_ = false;
    parent_ = nullptr;
    helmet_ = nullptr;
    stateApplied_ = false;
}

void NautyAnimator::update()
{
    if (!active_ || !face_.isReady())
    {
        return;
    }

    const NautyState state =
        nautyBehavior.getState();

    if (!stateApplied_ || state != renderedState_)
    {
        renderedState_ = state;
        stateApplied_ = true;

        applyState(state);

        Serial.printf(
            "[NautyAnimator] Mostrando estado: %s\n",
            nautyBehavior.getStateName()
        );
    }

    animateState(
        state,
        nautyBehavior.getStateElapsedTime()
    );

    face_.update();
}

void NautyAnimator::applyState(NautyState state)
{
    /*
     * Cada cambio comienza desde una cara conocida. De esta forma
     * ninguna emoción hereda accidentalmente los ojos, la boca o
     * la antena de la expresión anterior.
     */
    face_.reset();

    switch (state)
    {
        case NautyState::IDLE:
            break;

        case NautyState::BLINK:
            break;

        case NautyState::LOOK_LEFT:
            face_.setLookDirection(-100, 0);
            face_.setAntennaTilt(-8);
            break;

        case NautyState::LOOK_RIGHT:
            face_.setLookDirection(100, 0);
            face_.setAntennaTilt(8);
            break;

        case NautyState::HAPPY:
            face_.setEyesOpen(82);
            face_.setEyebrowLeft(-6);
            face_.setEyebrowRight(-6);
            face_.setMouthShape(NautyMouthShape::HAPPY);
            face_.setAntennaTilt(10);
            break;

        case NautyState::SAD:
            face_.setEyesOpen(62);
            face_.setLookDirection(0, 55);
            face_.setEyebrowLeft(10);
            face_.setEyebrowRight(10);
            face_.setMouthShape(NautyMouthShape::SAD);
            face_.setAntennaTilt(-18);
            break;

        case NautyState::ANGRY:
            face_.setEyesOpen(55);
            face_.setLookDirection(0, 20);
            face_.setEyebrowLeft(-12);
            face_.setEyebrowRight(-12);
            face_.setMouthShape(NautyMouthShape::SAD);
            face_.setAntennaTilt(0);
            break;

        case NautyState::CURIOUS:
            face_.setLookDirection(65, -45);
            face_.setEyebrowLeft(-14);
            face_.setEyebrowRight(6);
            face_.setMouthShape(NautyMouthShape::SURPRISED);
            face_.setAntennaTilt(24);
            break;

        case NautyState::UNCERTAIN:
            face_.setEyesOpen(75);
            face_.setEyebrowLeft(-4);
            face_.setEyebrowRight(9);
            face_.setMouthShape(NautyMouthShape::SAD);
            break;

        case NautyState::TIRED:
            face_.setEyesOpen(42);
            face_.setLookDirection(0, 50);
            face_.setEyebrowLeft(8);
            face_.setEyebrowRight(8);
            face_.setMouthShape(NautyMouthShape::NEUTRAL);
            face_.setAntennaTilt(-12);
            break;

        case NautyState::SLEEPY:
            face_.setEyesOpen(22);
            face_.setLookDirection(0, 65);
            face_.setEyebrowLeft(12);
            face_.setEyebrowRight(12);
            face_.setMouthShape(NautyMouthShape::NEUTRAL);
            face_.setAntennaTilt(-22);
            break;

        case NautyState::SLEEPING:
            face_.setEyesOpen(0);
            face_.setEyebrowLeft(14);
            face_.setEyebrowRight(14);
            face_.setMouthShape(NautyMouthShape::NEUTRAL);
            face_.setAntennaTilt(-28);
            break;

        case NautyState::CELEBRATING:
            face_.setEyesOpen(75);
            face_.setEyebrowLeft(-10);
            face_.setEyebrowRight(-10);
            face_.setMouthShape(NautyMouthShape::HAPPY);
            break;
    }
}

void NautyAnimator::animateState(
    NautyState state,
    uint32_t elapsedMs
)
{
    switch (state)
    {
        case NautyState::BLINK:
        {
            /* Cierra durante 90 ms y vuelve a abrir durante 90 ms. */
            const uint32_t phase = elapsedMs % 180;
            const uint8_t openness =
                phase < 90
                    ? static_cast<uint8_t>(100 - (phase * 100) / 90)
                    : static_cast<uint8_t>(((phase - 90) * 100) / 90);

            face_.setEyesOpen(openness);
            break;
        }

        case NautyState::UNCERTAIN:
        {
            /* La mirada duda de un lado a otro sin bloquear LVGL. */
            const uint32_t phase = elapsedMs % 1200;
            const int16_t look =
                phase < 600
                    ? static_cast<int16_t>(-55 + (phase * 110) / 600)
                    : static_cast<int16_t>(55 - ((phase - 600) * 110) / 600);

            face_.setLookDirection(look, 12);
            face_.setAntennaTilt(look / 3);
            break;
        }

        case NautyState::SLEEPING:
        {
            /* Respiración visual muy suave mediante la antena. */
            const uint32_t phase = elapsedMs % 2000;
            const int16_t movement =
                phase < 1000
                    ? static_cast<int16_t>((phase * 6) / 1000)
                    : static_cast<int16_t>(6 - ((phase - 1000) * 6) / 1000);

            face_.setAntennaTilt(-28 + movement);
            break;
        }

        case NautyState::CELEBRATING:
        {
            /* Movimiento alegre alternado de mirada y antena. */
            const bool alternate =
                ((elapsedMs / 220) % 2) == 0;

            face_.setLookDirection(
                alternate ? -45 : 45,
                -20
            );

            face_.setAntennaTilt(
                alternate ? -25 : 25
            );

            face_.setEyesOpen(
                alternate ? 72 : 90
            );
            break;
        }

        default:
            break;
    }
}

NautyFace &NautyAnimator::face()
{
    return face_;
}
