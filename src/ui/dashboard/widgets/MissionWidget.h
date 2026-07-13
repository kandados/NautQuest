#pragma once

#include <lvgl.h>

class MissionWidget
{
public:
    lv_obj_t* create(lv_obj_t* parent);
};

extern MissionWidget NQMissionWidget;