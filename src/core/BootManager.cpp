#include "BootManager.h"

#include <Arduino.h>

#include "drivers/display/NQDisplay.h"
#include "core/ScreenManager.h"

void BootManager::begin()
{
    Serial.println("[BootManager] Starting NautQuest boot sequence");

    bootStartTime = millis();

    // Inicializa la pantalla AMOLED
    NQDisplay.begin();

    // Muestra la pantalla de arranque
    NQDisplay.showBootScreen();

    Serial.println("[BootManager] Display initialized");
}

void BootManager::update()
{
    // Espera 3 segundos antes de cambiar al escritorio
    if (!bootCompleted && millis() - bootStartTime >= 3000)
    {
        bootCompleted = true;

        Serial.println("[BootManager] Boot completed");
        Serial.println("[BootManager] Requesting Desktop screen...");

        // Pide al ScreenManager que cambie al escritorio
        NQScreen.show(ScreenID::Desktop);

        Serial.println("[BootManager] Desktop request sent");
    }

    // Mantiene LVGL funcionando
    NQDisplay.update();
}

BootManager NQBoot;