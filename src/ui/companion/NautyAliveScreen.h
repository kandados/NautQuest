#pragma once

#include <lvgl.h>

class NautyAliveScreen
{
public:
    void show();

private:
    static void backEvent(lv_event_t *event);
};

extern NautyAliveScreen NQNautyAlive;
