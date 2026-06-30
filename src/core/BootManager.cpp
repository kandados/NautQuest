#include "BootManager.h"

#include <Arduino.h>

#include "drivers/display/NQDisplay.h"
#include "core/ScreenManager.h"
#include "core/MissionManager.h"

void BootManager::begin()
{
    Serial.println("[BootManager] Iniciando secuencia de arranque NautQuest");

    NQDisplay.begin();
    NQMission.begin();

    phase = BootPhase::Mascot;
    phaseStartTime = millis();

    NQDisplay.showMascotBootScreen();

    Serial.println("[BootManager] Fase inicial: Mascota");
}

void BootManager::update()
{
    unsigned long now = millis();

    if (phase == BootPhase::Mascot && now - phaseStartTime >= 2500)
    {
        phase = BootPhase::Logo;
        phaseStartTime = now;

        Serial.println("[BootManager] Fase: Logo");
        NQDisplay.showLogoBootScreen();
    }
    else if (phase == BootPhase::Logo && now - phaseStartTime >= 2500)
    {
        phase = BootPhase::Adventure;
        phaseStartTime = now;

        Serial.println("[BootManager] Fase: Mensaje de aventura");
        NQDisplay.showAdventureBootScreen();
    }
    else if (phase == BootPhase::Adventure && now - phaseStartTime >= 2500)
    {
        phase = BootPhase::Completed;

        Serial.println("[BootManager] Arranque completado");
        Serial.println("[BootManager] Mostrando Inicio");

        NQScreen.show(ScreenID::Desktop);
    }

    NQDisplay.update();
}

BootManager NQBoot;