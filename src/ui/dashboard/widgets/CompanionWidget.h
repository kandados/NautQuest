#pragma once

#include <lvgl.h>

class CompanionWidget
{
public:
    lv_obj_t* create(lv_obj_t* parent);
};

extern CompanionWidget NQCompanionWidget;