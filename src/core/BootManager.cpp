#include "BootManager.h"

#include <Arduino.h>
#include "core/NotificationManager.h"
#include "core/ExperienceManager.h"
#include "core/NavigationManager.h"
#include "core/theme/ThemeManager.h"
#include "drivers/display/NQDisplay.h"
#include "core/ScreenManager.h"
#include "core/MissionManager.h"

void BootManager::begin()
{
    Serial.print("[BootManager] Inicio ms=");
    Serial.println(millis());

    NQDisplay.begin();
    NQMission.begin();
    NQExperience.begin();
    NQNotification.begin();
    NQTheme.begin();
    NQNavigation.begin();

    phase = BootPhase::Logo;
    phaseStartTime = millis();

    NQDisplay.showLogoBootScreen();

    Serial.print("[BootManager] Fase inicial: Logo ms=");
    Serial.println(millis());
}

void BootManager::update()
{
    unsigned long now = millis();

    if (phase == BootPhase::Logo && now - phaseStartTime >= 2500)
    {
        phase = BootPhase::Adventure;
        phaseStartTime = now;

        Serial.print("[BootManager] Fase: Mensaje de aventura ms=");
        Serial.println(millis());

        NQDisplay.showAdventureBootScreen();
    }
    else if (phase == BootPhase::Adventure && now - phaseStartTime >= 2500)
    {
        phase = BootPhase::Completed;

        Serial.print("[BootManager] Arranque completado ms=");
        Serial.println(millis());

        Serial.println("[BootManager] Mostrando Inicio");
        NQScreen.show(ScreenID::Desktop);
    }
    NQNotification.update();
    NQDisplay.update();
}

BootManager NQBoot;