#pragma once

#include <lvgl.h>

class XPWidget
{
public:
    lv_obj_t* create(lv_obj_t* parent);
};

extern XPWidget NQXPWidget;