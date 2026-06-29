#include "ScreenManager.h"

#include <Arduino.h>

#include "ui/desktop/DesktopScreen.h"
#include "ui/settings/SettingsScreen.h"

void ScreenManager::show(ScreenID screen)
{
    if (screen == activeScreen)
    {
        return;
    }

    lastScreen = activeScreen;
    activeScreen = screen;

    Serial.println("--------------------------------");
    Serial.print("[ScreenManager] Previous: ");
    Serial.println(screenName(lastScreen));

    Serial.print("[ScreenManager] Current : ");
    Serial.println(screenName(activeScreen));
    Serial.println("--------------------------------");

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
            Serial.println("[ScreenManager] Showing SettingsScreen");
            NQSettings.show();
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

ScreenID ScreenManager::previousScreen() const
{
    return lastScreen;
}

const char* ScreenManager::currentScreenName() const
{
    return screenName(activeScreen);
}

const char* ScreenManager::previousScreenName() const
{
    return screenName(lastScreen);
}

const char* ScreenManager::screenName(ScreenID screen) const
{
    switch (screen)
    {
        case ScreenID::Boot:
            return "Boot";

        case ScreenID::Desktop:
            return "Desktop";

        case ScreenID::Settings:
            return "Settings";

        case ScreenID::Compass:
            return "Compass";

        case ScreenID::Calendar:
            return "Calendar";

        case ScreenID::AI:
            return "AI";

        default:
            return "Unknown";
    }
}

ScreenManager NQScreen;