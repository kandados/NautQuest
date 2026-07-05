#include "DeveloperScreen.h"

#include <Arduino.h>
#include <lvgl.h>

#include "core/NavigationManager.h"
#include "core/theme/ThemeManager.h"

#define CAMERA_UART_RX 44
#define CAMERA_UART_TX 43

static HardwareSerial CameraSerial(1);
static bool cameraUartStarted = false;
static lv_obj_t *cameraStatusLabel = nullptr;

static void ensureCameraUartStarted()
{
    if (!cameraUartStarted)
    {
        CameraSerial.begin(115200, SERIAL_8N1, CAMERA_UART_RX, CAMERA_UART_TX);
        cameraUartStarted = true;

        Serial.println("[DeveloperScreen] Camera UART iniciado");
    }
}

static String sendCameraCommand(const char* command)
{
    ensureCameraUartStarted();

    while (CameraSerial.available())
    {
        CameraSerial.read();
    }

    Serial.print("[DeveloperScreen] Enviando comando camara: ");
    Serial.println(command);

    CameraSerial.println(command);

    unsigned long startTime = millis();
    String response = "";

    while (millis() - startTime < 1000)
    {
        if (CameraSerial.available())
        {
            response = CameraSerial.readStringUntil('\n');
            response.trim();
            break;
        }

        delay(10);
    }

    Serial.print("[DeveloperScreen] Respuesta camara: ");
    Serial.println(response);

    return response;
}

static void backButtonEventHandler(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[DeveloperScreen] Volver pulsado");
        NQNavigation.goBack();
    }
}

static void testCameraButtonEventHandler(lv_event_t *event)
{
    if (lv_event_get_code(event) != LV_EVENT_CLICKED)
    {
        return;
    }

    String response = sendCameraCommand("PING");

    if (response == "PONG")
    {
        if (cameraStatusLabel != nullptr)
        {
            lv_label_set_text(cameraStatusLabel, "Camara OK");
        }

        Serial.println("[DeveloperScreen] Modulo camara conectado");
    }
    else
    {
        if (cameraStatusLabel != nullptr)
        {
            lv_label_set_text(cameraStatusLabel, "Sin respuesta");
        }

        Serial.println("[DeveloperScreen] Modulo camara sin respuesta");
    }
}

static void cameraStatusButtonEventHandler(lv_event_t *event)
{
    if (lv_event_get_code(event) != LV_EVENT_CLICKED)
    {
        return;
    }

    String response = sendCameraCommand("CAMERA?");

    if (cameraStatusLabel != nullptr)
    {
        if (response.length() > 0)
        {
            lv_label_set_text(cameraStatusLabel, response.c_str());
        }
        else
        {
            lv_label_set_text(cameraStatusLabel, "Sin respuesta");
        }
    }
}

void DeveloperScreen::show()
{
    ensureCameraUartStarted();

    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(lv_scr_act(), NQTheme.background(), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "DEVELOPER");
    lv_obj_set_style_text_color(title, NQTheme.text(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_30, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -165);

    lv_obj_t *subtitle = lv_label_create(lv_scr_act());
    lv_label_set_text(subtitle, "Herramientas internas");
    lv_obj_set_style_text_color(subtitle, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_16, 0);
    lv_obj_align(subtitle, LV_ALIGN_CENTER, 0, -130);

    lv_obj_t *missions = lv_label_create(lv_scr_act());
    lv_label_set_text(missions, "> Misiones");
    lv_obj_set_style_text_color(missions, NQTheme.text(), 0);
    lv_obj_set_style_text_font(missions, &lv_font_montserrat_22, 0);
    lv_obj_align(missions, LV_ALIGN_CENTER, 0, -80);

    lv_obj_t *experience = lv_label_create(lv_scr_act());
    lv_label_set_text(experience, "> Experiencia");
    lv_obj_set_style_text_color(experience, NQTheme.text(), 0);
    lv_obj_set_style_text_font(experience, &lv_font_montserrat_22, 0);
    lv_obj_align(experience, LV_ALIGN_CENTER, 0, -45);

    lv_obj_t *system = lv_label_create(lv_scr_act());
    lv_label_set_text(system, "> Sistema");
    lv_obj_set_style_text_color(system, NQTheme.text(), 0);
    lv_obj_set_style_text_font(system, &lv_font_montserrat_22, 0);
    lv_obj_align(system, LV_ALIGN_CENTER, 0, -10);

    lv_obj_t *hardware = lv_label_create(lv_scr_act());
    lv_label_set_text(hardware, "> Hardware");
    lv_obj_set_style_text_color(hardware, NQTheme.text(), 0);
    lv_obj_set_style_text_font(hardware, &lv_font_montserrat_22, 0);
    lv_obj_align(hardware, LV_ALIGN_CENTER, 0, 25);

    lv_obj_t *testCameraButton = lv_btn_create(lv_scr_act());
    lv_obj_set_size(testCameraButton, 185, 34);
    lv_obj_align(testCameraButton, LV_ALIGN_CENTER, 0, 72);
    lv_obj_add_event_cb(testCameraButton, testCameraButtonEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *testCameraLabel = lv_label_create(testCameraButton);
    lv_label_set_text(testCameraLabel, "Test Camara");
    lv_obj_set_style_text_font(testCameraLabel, &lv_font_montserrat_14, 0);
    lv_obj_center(testCameraLabel);

    lv_obj_t *cameraStatusButton = lv_btn_create(lv_scr_act());
    lv_obj_set_size(cameraStatusButton, 185, 34);
    lv_obj_align(cameraStatusButton, LV_ALIGN_CENTER, 0, 112);
    lv_obj_add_event_cb(cameraStatusButton, cameraStatusButtonEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *cameraStatusButtonLabel = lv_label_create(cameraStatusButton);
    lv_label_set_text(cameraStatusButtonLabel, "Estado Camara");
    lv_obj_set_style_text_font(cameraStatusButtonLabel, &lv_font_montserrat_14, 0);
    lv_obj_center(cameraStatusButtonLabel);

    cameraStatusLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(cameraStatusLabel, "Camara: pendiente");
    lv_obj_set_style_text_color(cameraStatusLabel, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_text_font(cameraStatusLabel, &lv_font_montserrat_14, 0);
    lv_obj_align(cameraStatusLabel, LV_ALIGN_CENTER, 0, 150);

    lv_obj_t *backButton = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(backButton, NQTheme.backButton(), 0);
    lv_obj_set_size(backButton, 180, 34);
    lv_obj_align(backButton, LV_ALIGN_CENTER, 0, 190);
    lv_obj_add_event_cb(backButton, backButtonEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *backLabel = lv_label_create(backButton);
    lv_label_set_text(backLabel, "Volver");
    lv_obj_set_style_text_font(backLabel, &lv_font_montserrat_16, 0);
    lv_obj_center(backLabel);

    lv_timer_handler();
}

DeveloperScreen NQDeveloper;