#pragma once

#include <lvgl.h>

class Nauty
{
public:

    void create(lv_obj_t *parent);

    void idle();

    void blink();

    void happy();

    void thinking();

    void sleeping();

    void surprised();

    void sad();

    void update();

private:

    lv_obj_t *root=nullptr;

    lv_obj_t *helmet=nullptr;

    lv_obj_t *visor=nullptr;

    lv_obj_t *leftEye=nullptr;

    lv_obj_t *rightEye=nullptr;

    lv_obj_t *mouth=nullptr;

    lv_obj_t *antenna=nullptr;
};

extern Nauty NQNauty;