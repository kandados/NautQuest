#include "BootManager.h"

#include <Arduino.h>

#include "drivers/display/NQDisplay.h"
#include "ui/desktop/DesktopScreen.h"

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
        Serial.println("[BootManager] Desktop loading...");

        // Cambia al escritorio
        NQDesktop.show();

        Serial.println("[BootManager] Desktop loaded");
    }

    // Mantiene LVGL funcionando
    NQDisplay.update();
}

BootManager NQBoot;