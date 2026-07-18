#pragma once

#include <lvgl.h>

#include "core/companion/NautyFace.h"
#include "nauty/NautyBehavior.h"

class NautyAnimator
{
public:
    void begin(lv_obj_t *parent);
    void stop();
    void update();

    NautyFace &face();

private:
    void applyState(NautyState state);
    void animateState(NautyState state, uint32_t elapsedMs);

    lv_obj_t *parent_ = nullptr;
    lv_obj_t *helmet_ = nullptr;

    NautyFace face_;

    NautyState renderedState_ = NautyState::IDLE;
    bool stateApplied_ = false;
    bool active_ = false;
};

extern NautyAnimator NQNautyAnimator;
