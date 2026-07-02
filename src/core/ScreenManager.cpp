#include "ScreenManager.h"

#include <Arduino.h>

#include "ui/desktop/DesktopScreen.h"
#include "ui/settings/SettingsScreen.h"
#include "ui/missions/MissionsScreen.h"
#include "ui/missions/MissionDetailScreen.h"
#include "ui/logbook/LogbookScreen.h"
#include "ui/oracle/OracleScreen.h"

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

        case ScreenID::Missions:
            Serial.println("[ScreenManager] Showing MissionsScreen");
            NQMissions.show();
            break;

        case ScreenID::MissionDetail:
            Serial.println("[ScreenManager] Showing MissionDetailScreen");
            NQMissionDetail.show();
            break;

        case ScreenID::Logbook:
            Serial.println("[ScreenManager] Showing LogbookScreen");
            NQLogbook.show();
            break;

        case ScreenID::Oracle:
            Serial.println("[ScreenManager] Showing OracleScreen");
            NQOracle.show();
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
            return "Inicio";

        case ScreenID::Settings:
            return "Ajustes";

        case ScreenID::Missions:
            return "Misiones";

        case ScreenID::MissionDetail:
            return "Detalle mision";

        case ScreenID::Logbook:
            return "Bitacora";

        case ScreenID::Oracle:
            return "Oraculo";

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