#pragma once

#include <lvgl.h>

enum class NautyMouthShape : uint8_t
{
    NEUTRAL,
    HAPPY,
    SAD,
    SURPRISED
};

class NautyFace
{
public:
    void begin(lv_obj_t *parent);
    void update();

    void reset();

    void setEyesOpen(uint8_t percentage);
    void setLookDirection(int16_t x, int16_t y);

    void setEyebrowLeft(int16_t offsetY);
    void setEyebrowRight(int16_t offsetY);

    void setMouthShape(NautyMouthShape shape);
    void setAntennaTilt(int16_t offsetX);

    bool isReady() const;

private:
    void createEye(
        lv_obj_t *parent,
        lv_obj_t **eye,
        lv_obj_t **pupil,
        lv_obj_t **eyelid,
        lv_coord_t x,
        lv_coord_t y
    );

    void updateEyelids();
    void updatePupils();
    void updateEyebrows();
    void updateMouth();
    void updateAntenna();

    lv_obj_t *root_ = nullptr;

    lv_obj_t *leftEye_ = nullptr;
    lv_obj_t *rightEye_ = nullptr;

    lv_obj_t *leftPupil_ = nullptr;
    lv_obj_t *rightPupil_ = nullptr;

    lv_obj_t *leftEyelid_ = nullptr;
    lv_obj_t *rightEyelid_ = nullptr;

    lv_obj_t *leftEyebrow_ = nullptr;
    lv_obj_t *rightEyebrow_ = nullptr;

    lv_obj_t *mouth_ = nullptr;

    lv_obj_t *antennaStem_ = nullptr;
    lv_obj_t *antennaTip_ = nullptr;

    uint8_t eyesOpenPercentage_ = 100;

    int16_t lookX_ = 0;
    int16_t lookY_ = 0;

    int16_t leftEyebrowOffsetY_ = 0;
    int16_t rightEyebrowOffsetY_ = 0;

    int16_t antennaOffsetX_ = 0;

    NautyMouthShape mouthShape_ =
        NautyMouthShape::NEUTRAL;

    lv_point_t leftEyebrowPoints_[2];
    lv_point_t rightEyebrowPoints_[2];

    lv_point_t mouthPoints_[3];
    lv_point_t antennaPoints_[2];
};