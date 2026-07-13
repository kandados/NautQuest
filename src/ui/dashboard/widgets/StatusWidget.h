#pragma once

#include <lvgl.h>

class StatusWidget
{
public:
    lv_obj_t* create(lv_obj_t* parent);
};

extern StatusWidget NQStatusWidget;