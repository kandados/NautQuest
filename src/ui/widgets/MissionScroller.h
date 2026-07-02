#pragma once

#include <lvgl.h>

class MissionScroller
{
public:
    void create(lv_obj_t *parent);
    void clear();

    void addItem(const char *text, bool completed);
    void addPlainItem(const char *text);

    void refresh();

private:
    static constexpr int MAX_ITEMS = 20;

    lv_obj_t *container = nullptr;

    lv_obj_t *labels[MAX_ITEMS];
    lv_obj_t *states[MAX_ITEMS];

    int itemCount = 0;

    void addInternalItem(const char *text, const char *state, bool completed);

    static void scrollEvent(lv_event_t *e);
};

extern MissionScroller NQMissionScroller;