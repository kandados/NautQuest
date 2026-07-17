#pragma once

#include <lvgl.h>

#include "core/companion/NautyFace.h"

class NautyAnimator
{
public:
    void begin(lv_obj_t *parent);
    void update();

    NautyFace &face();

private:
    lv_obj_t *parent_ = nullptr;
    lv_obj_t *helmet_ = nullptr;

    NautyFace face_;
};

extern NautyAnimator NQNautyAnimator;