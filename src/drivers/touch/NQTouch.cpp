#include "NQTouch.h"

#include <Arduino.h>
#include <Wire.h>

#include "TouchDrvCSTXXX.hpp"

#define TOUCH_SDA 15
#define TOUCH_SCL 14
#define TOUCH_IRQ 11
#define TOUCH_RST 40
#define TOUCH_ADDR 0x5A

#define TOUCH_WIDTH 466
#define TOUCH_HEIGHT 466

static TouchDrvCST92xx touch;
static int16_t touchX[5];
static int16_t touchY[5];

static volatile bool touchInterrupt = false;

static void IRAM_ATTR onTouchInterrupt()
{
    touchInterrupt = true;
}

bool TouchDriver::begin()
{
    Serial.println("[NQTouch] Initializing CST9217 touch...");

    pinMode(TOUCH_RST, OUTPUT);
    digitalWrite(TOUCH_RST, LOW);
    delay(30);
    digitalWrite(TOUCH_RST, HIGH);
    delay(100);


    touch.setPins(TOUCH_RST, TOUCH_IRQ);

    bool result = touch.begin(Wire, TOUCH_ADDR, TOUCH_SDA, TOUCH_SCL);

    if (!result)
    {
        Serial.println("[NQTouch] Touch not found");
        initialized = false;
        return false;
    }

    Serial.print("[NQTouch] Touch ready. Model: ");
    Serial.println(touch.getModelName());

    touch.sleep();
    touch.reset();

    touch.setMaxCoordinates(TOUCH_WIDTH, TOUCH_HEIGHT);
    touch.setMirrorXY(true, true);

    pinMode(TOUCH_IRQ, INPUT_PULLUP);
    attachInterrupt(TOUCH_IRQ, onTouchInterrupt, FALLING);

    Serial.println("[NQTouch] Touch IRQ configured");
    Serial.println("[NQTouch] Touch coordinates configured");

    initialized = true;
    return true;
}

void TouchDriver::read(lv_indev_drv_t *driver, lv_indev_data_t *data)
{
    if (!initialized)
    {
        data->state = LV_INDEV_STATE_REL;
        return;
    }

    if (!touchInterrupt)
    {
        data->state = LV_INDEV_STATE_REL;
        return;
    }

    touchInterrupt = false;

    uint8_t touched = touch.getPoint(touchX, touchY, touch.getSupportTouchPoint());

    if (touched > 0)
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX[0];
        data->point.y = touchY[0];
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

TouchDriver NQTouch;