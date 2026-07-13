#pragma once

#include <lvgl.h>

class BottomBarWidget
{
public:
    lv_obj_t* create(lv_obj_t* parent);
};

extern BottomBarWidget NQBottomBarWidget;