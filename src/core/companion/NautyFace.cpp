#include "NautyFace.h"

#include <Arduino.h>

namespace
{
    constexpr lv_coord_t SCREEN_SIZE = 466;

    constexpr lv_coord_t EYE_WIDTH = 88;
    constexpr lv_coord_t EYE_HEIGHT = 62;

    constexpr lv_coord_t LEFT_EYE_X = 126;
    constexpr lv_coord_t RIGHT_EYE_X = 252;
    constexpr lv_coord_t EYE_Y = 188;

    constexpr lv_coord_t PUPIL_SIZE = 26;
    constexpr lv_coord_t PUPIL_MAX_X = 20;
    constexpr lv_coord_t PUPIL_MAX_Y = 12;

    constexpr lv_coord_t EYEBROW_Y = 160;

    constexpr lv_coord_t MOUTH_X = 183;
    constexpr lv_coord_t MOUTH_Y = 282;

    constexpr lv_coord_t ANTENNA_BASE_X = 233;
    constexpr lv_coord_t ANTENNA_BASE_Y = 112;
    constexpr lv_coord_t ANTENNA_LENGTH = 48;

    constexpr uint32_t FACE_WHITE = 0xF5F7FA;
    constexpr uint32_t FACE_DARK = 0x111820;
    constexpr uint32_t FACE_BLUE = 0x28B8FF;
}

void NautyFace::begin(lv_obj_t *parent)
{
    if (parent == nullptr)
    {
        Serial.println(
            "[NautyFace] ERROR: parent nulo"
        );

        return;
    }

    root_ = lv_obj_create(parent);

    lv_obj_set_size(
        root_,
        SCREEN_SIZE,
        SCREEN_SIZE
    );

    lv_obj_center(root_);

    lv_obj_set_style_bg_opa(
        root_,
        LV_OPA_TRANSP,
        0
    );

    lv_obj_set_style_border_width(
        root_,
        0,
        0
    );

    lv_obj_set_style_pad_all(
        root_,
        0,
        0
    );

    lv_obj_clear_flag(
        root_,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_clear_flag(
        root_,
        LV_OBJ_FLAG_CLICKABLE
    );

    /*
     * Antena.
     */
    antennaStem_ = lv_line_create(root_);

    lv_obj_set_style_line_color(
        antennaStem_,
        lv_color_hex(FACE_BLUE),
        0
    );

    lv_obj_set_style_line_width(
        antennaStem_,
        7,
        0
    );

    lv_obj_set_style_line_rounded(
        antennaStem_,
        true,
        0
    );

    antennaTip_ = lv_obj_create(root_);

    lv_obj_set_size(
        antennaTip_,
        22,
        22
    );

    lv_obj_set_style_radius(
        antennaTip_,
        LV_RADIUS_CIRCLE,
        0
    );

    lv_obj_set_style_bg_color(
        antennaTip_,
        lv_color_hex(FACE_BLUE),
        0
    );

    lv_obj_set_style_bg_opa(
        antennaTip_,
        LV_OPA_COVER,
        0
    );

    lv_obj_set_style_border_width(
        antennaTip_,
        0,
        0
    );

    /*
     * Cejas.
     */
    leftEyebrow_ = lv_line_create(root_);
    rightEyebrow_ = lv_line_create(root_);

    lv_obj_set_style_line_color(
        leftEyebrow_,
        lv_color_hex(FACE_WHITE),
        0
    );

    lv_obj_set_style_line_color(
        rightEyebrow_,
        lv_color_hex(FACE_WHITE),
        0
    );

    lv_obj_set_style_line_width(
        leftEyebrow_,
        8,
        0
    );

    lv_obj_set_style_line_width(
        rightEyebrow_,
        8,
        0
    );

    lv_obj_set_style_line_rounded(
        leftEyebrow_,
        true,
        0
    );

    lv_obj_set_style_line_rounded(
        rightEyebrow_,
        true,
        0
    );

    /*
     * Ojos completos.
     */
    createEye(
        root_,
        &leftEye_,
        &leftPupil_,
        &leftEyelid_,
        LEFT_EYE_X,
        EYE_Y
    );

    createEye(
        root_,
        &rightEye_,
        &rightPupil_,
        &rightEyelid_,
        RIGHT_EYE_X,
        EYE_Y
    );

    /*
     * Boca.
     */
    mouth_ = lv_line_create(root_);

    lv_obj_set_style_line_color(
        mouth_,
        lv_color_hex(FACE_WHITE),
        0
    );

    lv_obj_set_style_line_width(
        mouth_,
        9,
        0
    );

    lv_obj_set_style_line_rounded(
        mouth_,
        true,
        0
    );

    reset();

    Serial.println(
        "[NautyFace] Elementos faciales creados"
    );
}

void NautyFace::createEye(
    lv_obj_t *parent,
    lv_obj_t **eye,
    lv_obj_t **pupil,
    lv_obj_t **eyelid,
    lv_coord_t x,
    lv_coord_t y
)
{
    /*
     * Fondo del ojo.
     */
    *eye = lv_obj_create(parent);

    lv_obj_set_size(
        *eye,
        EYE_WIDTH,
        EYE_HEIGHT
    );

    lv_obj_set_pos(
        *eye,
        x,
        y
    );

    lv_obj_set_style_radius(
        *eye,
        LV_RADIUS_CIRCLE,
        0
    );

    lv_obj_set_style_bg_color(
        *eye,
        lv_color_hex(FACE_WHITE),
        0
    );

    lv_obj_set_style_bg_opa(
        *eye,
        LV_OPA_COVER,
        0
    );

    lv_obj_set_style_border_width(
        *eye,
        0,
        0
    );

    lv_obj_set_style_pad_all(
        *eye,
        0,
        0
    );

    lv_obj_clear_flag(
        *eye,
        LV_OBJ_FLAG_SCROLLABLE
    );

    /*
     * Pupila.
     */
    *pupil = lv_obj_create(*eye);

    lv_obj_set_size(
        *pupil,
        PUPIL_SIZE,
        PUPIL_SIZE
    );

    lv_obj_set_style_radius(
        *pupil,
        LV_RADIUS_CIRCLE,
        0
    );

    lv_obj_set_style_bg_color(
        *pupil,
        lv_color_hex(FACE_DARK),
        0
    );

    lv_obj_set_style_bg_opa(
        *pupil,
        LV_OPA_COVER,
        0
    );

    lv_obj_set_style_border_width(
        *pupil,
        0,
        0
    );

    lv_obj_center(*pupil);

    /*
     * Párpado.
     *
     * Se dibuja por encima del ojo y aumenta su altura
     * para cerrar progresivamente el ojo.
     */
    *eyelid = lv_obj_create(*eye);

    lv_obj_set_width(
        *eyelid,
        EYE_WIDTH
    );

    lv_obj_set_height(
        *eyelid,
        0
    );

    lv_obj_set_pos(
        *eyelid,
        0,
        0
    );

    lv_obj_set_style_radius(
        *eyelid,
        LV_RADIUS_CIRCLE,
        0
    );

    lv_obj_set_style_bg_color(
        *eyelid,
        lv_color_hex(FACE_DARK),
        0
    );

    lv_obj_set_style_bg_opa(
        *eyelid,
        LV_OPA_COVER,
        0
    );

    lv_obj_set_style_border_width(
        *eyelid,
        0,
        0
    );

    lv_obj_move_foreground(*eyelid);
}

void NautyFace::reset()
{
    eyesOpenPercentage_ = 100;

    lookX_ = 0;
    lookY_ = 0;

    leftEyebrowOffsetY_ = 0;
    rightEyebrowOffsetY_ = 0;

    antennaOffsetX_ = 0;

    mouthShape_ =
        NautyMouthShape::NEUTRAL;

    updateEyelids();
    updatePupils();
    updateEyebrows();
    updateMouth();
    updateAntenna();
}

void NautyFace::update()
{
    /*
     * Las futuras animaciones temporizadas se actualizarán aquí.
     *
     * Los elementos ya son completamente independientes.
     */
}

void NautyFace::setEyesOpen(
    uint8_t percentage
)
{
    eyesOpenPercentage_ =
        percentage > 100
            ? 100
            : percentage;

    updateEyelids();
}

void NautyFace::setLookDirection(
    int16_t x,
    int16_t y
)
{
    lookX_ = constrain(
        x,
        -100,
        100
    );

    lookY_ = constrain(
        y,
        -100,
        100
    );

    updatePupils();
}

void NautyFace::setEyebrowLeft(
    int16_t offsetY
)
{
    leftEyebrowOffsetY_ =
        constrain(
            offsetY,
            -30,
            30
        );

    updateEyebrows();
}

void NautyFace::setEyebrowRight(
    int16_t offsetY
)
{
    rightEyebrowOffsetY_ =
        constrain(
            offsetY,
            -30,
            30
        );

    updateEyebrows();
}

void NautyFace::setMouthShape(
    NautyMouthShape shape
)
{
    mouthShape_ = shape;
    updateMouth();
}

void NautyFace::setAntennaTilt(
    int16_t offsetX
)
{
    antennaOffsetX_ =
        constrain(
            offsetX,
            -35,
            35
        );

    updateAntenna();
}

bool NautyFace::isReady() const
{
    return root_ != nullptr;
}

void NautyFace::updateEyelids()
{
    if (
        leftEyelid_ == nullptr ||
        rightEyelid_ == nullptr
    )
    {
        return;
    }

    const lv_coord_t closedHeight =
        static_cast<lv_coord_t>(
            EYE_HEIGHT -
            (
                EYE_HEIGHT *
                eyesOpenPercentage_
            ) /
            100
        );

    lv_obj_set_height(
        leftEyelid_,
        closedHeight
    );

    lv_obj_set_height(
        rightEyelid_,
        closedHeight
    );

    lv_obj_move_foreground(
        leftEyelid_
    );

    lv_obj_move_foreground(
        rightEyelid_
    );
}

void NautyFace::updatePupils()
{
    if (
        leftPupil_ == nullptr ||
        rightPupil_ == nullptr
    )
    {
        return;
    }

    const lv_coord_t pupilOffsetX =
        static_cast<lv_coord_t>(
            (
                PUPIL_MAX_X *
                lookX_
            ) /
            100
        );

    const lv_coord_t pupilOffsetY =
        static_cast<lv_coord_t>(
            (
                PUPIL_MAX_Y *
                lookY_
            ) /
            100
        );

    lv_obj_align(
        leftPupil_,
        LV_ALIGN_CENTER,
        pupilOffsetX,
        pupilOffsetY
    );

    lv_obj_align(
        rightPupil_,
        LV_ALIGN_CENTER,
        pupilOffsetX,
        pupilOffsetY
    );
}

void NautyFace::updateEyebrows()
{
    if (
        leftEyebrow_ == nullptr ||
        rightEyebrow_ == nullptr
    )
    {
        return;
    }

    leftEyebrowPoints_[0] =
    {
        LEFT_EYE_X + 8,
        EYEBROW_Y + leftEyebrowOffsetY_
    };

    leftEyebrowPoints_[1] =
    {
        LEFT_EYE_X + EYE_WIDTH - 8,
        EYEBROW_Y + leftEyebrowOffsetY_
    };

    rightEyebrowPoints_[0] =
    {
        RIGHT_EYE_X + 8,
        EYEBROW_Y + rightEyebrowOffsetY_
    };

    rightEyebrowPoints_[1] =
    {
        RIGHT_EYE_X + EYE_WIDTH - 8,
        EYEBROW_Y + rightEyebrowOffsetY_
    };

    lv_line_set_points(
        leftEyebrow_,
        leftEyebrowPoints_,
        2
    );

    lv_line_set_points(
        rightEyebrow_,
        rightEyebrowPoints_,
        2
    );
}

void NautyFace::updateMouth()
{
    if (mouth_ == nullptr)
    {
        return;
    }

    switch (mouthShape_)
    {
        case NautyMouthShape::HAPPY:
            mouthPoints_[0] =
            {
                MOUTH_X,
                MOUTH_Y
            };

            mouthPoints_[1] =
            {
                MOUTH_X + 50,
                MOUTH_Y + 24
            };

            mouthPoints_[2] =
            {
                MOUTH_X + 100,
                MOUTH_Y
            };
            break;

        case NautyMouthShape::SAD:
            mouthPoints_[0] =
            {
                MOUTH_X,
                MOUTH_Y + 24
            };

            mouthPoints_[1] =
            {
                MOUTH_X + 50,
                MOUTH_Y
            };

            mouthPoints_[2] =
            {
                MOUTH_X + 100,
                MOUTH_Y + 24
            };
            break;

        case NautyMouthShape::SURPRISED:
            mouthPoints_[0] =
            {
                MOUTH_X + 35,
                MOUTH_Y
            };

            mouthPoints_[1] =
            {
                MOUTH_X + 50,
                MOUTH_Y + 28
            };

            mouthPoints_[2] =
            {
                MOUTH_X + 65,
                MOUTH_Y
            };
            break;

        case NautyMouthShape::NEUTRAL:
        default:
            mouthPoints_[0] =
            {
                MOUTH_X,
                MOUTH_Y + 10
            };

            mouthPoints_[1] =
            {
                MOUTH_X + 50,
                MOUTH_Y + 10
            };

            mouthPoints_[2] =
            {
                MOUTH_X + 100,
                MOUTH_Y + 10
            };
            break;
    }

    lv_line_set_points(
        mouth_,
        mouthPoints_,
        3
    );
}

void NautyFace::updateAntenna()
{
    if (
        antennaStem_ == nullptr ||
        antennaTip_ == nullptr
    )
    {
        return;
    }

    antennaPoints_[0] =
    {
        ANTENNA_BASE_X,
        ANTENNA_BASE_Y
    };

    antennaPoints_[1] =
    {
        ANTENNA_BASE_X + antennaOffsetX_,
        ANTENNA_BASE_Y - ANTENNA_LENGTH
    };

    lv_line_set_points(
        antennaStem_,
        antennaPoints_,
        2
    );

    lv_obj_set_pos(
        antennaTip_,
        ANTENNA_BASE_X +
            antennaOffsetX_ -
            11,
        ANTENNA_BASE_Y -
            ANTENNA_LENGTH -
            11
    );
}
