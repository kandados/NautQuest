#pragma once

#include <lvgl.h>

class TouchDriver {
public:
    bool begin();
    void read(lv_indev_drv_t *driver, lv_indev_data_t *data);

private:
    bool initialized = false;
};

extern TouchDriver NQTouch;