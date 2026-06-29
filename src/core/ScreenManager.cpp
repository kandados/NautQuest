#include "ScreenManager.h"

#include <Arduino.h>

#include "ui/desktop/DesktopScreen.h"

void ScreenManager::show(ScreenID screen)
{
    activeScreen = screen;

    switch (screen)
    {
        case ScreenID::Boot:
            Serial.println("[ScreenManager] Boot screen is handled by BootManager");
            break;

        case ScreenID::Desktop:
            Serial.println("[ScreenManager] Showing DesktopScreen");
            NQDesktop.show();
            break;

        case ScreenID::Settings:
            Serial.println("[ScreenManager] SettingsScreen not implemented yet");
            break;

        case ScreenID::Compass:
            Serial.println("[ScreenManager] CompassScreen not implemented yet");
            break;

        case ScreenID::Calendar:
            Serial.println("[ScreenManager] CalendarScreen not implemented yet");
            break;

        case ScreenID::AI:
            Serial.println("[ScreenManager] AIScreen not implemented yet");
            break;
    }
}

ScreenID ScreenManager::currentScreen() const
{
    return activeScreen;
}

ScreenManager NQScreen;