#include "BootManager.h"

#include <Arduino.h>

#include "drivers/display/NQDisplay.h"
#include "core/ScreenManager.h"

void BootManager::begin()
{
    Serial.println("[BootManager] Starting NautQuest boot sequence");

    bootStartTime = millis();

    NQDisplay.begin();
    NQDisplay.showBootScreen();

    Serial.println("[BootManager] Display initialized");
}

void BootManager::update()
{
    if (!bootCompleted && millis() - bootStartTime >= 3000)
    {
        bootCompleted = true;

        Serial.println("[BootManager] Boot completed");
        Serial.println("[BootManager] Requesting Desktop screen...");

        NQScreen.show(ScreenID::Desktop);

        Serial.println("[BootManager] Desktop request sent");

        Serial.print("[BootManager] Active screen: ");
        Serial.println(NQScreen.currentScreenName());

        Serial.print("[BootManager] Previous screen: ");
        Serial.println(NQScreen.previousScreenName());
    }

    NQDisplay.update();
}

BootManager NQBoot;