#include "ScreenManager.h"

#include <Arduino.h>

#include "ui/companion/NautyAliveScreen.h"
#include "ui/dashboard/DashboardScreen.h"
#include "ui/developer/DeveloperScreen.h"
#include "ui/logbook/LogbookScreen.h"
#include "ui/missions/MissionDetailScreen.h"
#include "ui/missions/MissionsScreen.h"
#include "ui/oracle/OracleScreen.h"
#include "ui/settings/SettingsScreen.h"
#include "ui/settings/display/DisplaySettingsScreen.h"
#include "ui/settings/sound/SoundSettingsScreen.h"

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
            Serial.println(
                "[ScreenManager] Boot gestionado por BootManager"
            );
            break;

        case ScreenID::Desktop:
            Serial.println(
                "[ScreenManager] Showing DashboardScreen / Inicio"
            );
            NQDashboard.show();
            break;

        case ScreenID::NautyAlive:
            Serial.println(
                "[ScreenManager] Showing NautyAliveScreen"
            );
            NQNautyAlive.show();
            break;

        case ScreenID::Settings:
            Serial.println(
                "[ScreenManager] Showing SettingsScreen"
            );
            NQSettings.show();
            break;

        case ScreenID::SettingsDisplay:
            Serial.println(
                "[ScreenManager] Showing DisplaySettingsScreen"
            );
            NQDisplaySettings.show();
            break;

        case ScreenID::SettingsSound:
            Serial.println(
                "[ScreenManager] Showing SoundSettingsScreen"
            );
            NQSoundSettings.show();
            break;

        case ScreenID::Developer:
            Serial.println(
                "[ScreenManager] Showing DeveloperScreen"
            );
            NQDeveloper.show();
            break;

        case ScreenID::Missions:
            Serial.println(
                "[ScreenManager] Showing MissionsScreen"
            );
            NQMissions.show();
            break;

        case ScreenID::MissionDetail:
            Serial.println(
                "[ScreenManager] Showing MissionDetailScreen"
            );
            NQMissionDetail.show();
            break;

        case ScreenID::Logbook:
            Serial.println(
                "[ScreenManager] Showing LogbookScreen"
            );
            NQLogbook.show();
            break;

        case ScreenID::Oracle:
            Serial.println(
                "[ScreenManager] Showing OracleScreen"
            );
            NQOracle.show();
            break;

        case ScreenID::Compass:
            Serial.println(
                "[ScreenManager] CompassScreen pendiente"
            );
            break;

        case ScreenID::Calendar:
            Serial.println(
                "[ScreenManager] CalendarScreen pendiente"
            );
            break;

        case ScreenID::AI:
            Serial.println(
                "[ScreenManager] IA integrada en Nauty Soul"
            );
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

const char *ScreenManager::currentScreenName() const
{
    return screenName(activeScreen);
}

const char *ScreenManager::previousScreenName() const
{
    return screenName(lastScreen);
}

const char *ScreenManager::nameOf(
    ScreenID screen
) const
{
    return screenName(screen);
}

const char *ScreenManager::screenName(
    ScreenID screen
) const
{
    switch (screen)
    {
        case ScreenID::Boot:
            return "Boot";

        case ScreenID::Desktop:
            return "Inicio";

        case ScreenID::NautyAlive:
            return "Nauty vivo";

        case ScreenID::Settings:
            return "Ajustes";

        case ScreenID::SettingsDisplay:
            return "Pantalla";

        case ScreenID::SettingsSound:
            return "Sonido";

        case ScreenID::Missions:
            return "Misiones";

        case ScreenID::MissionDetail:
            return "Detalle mision";

        case ScreenID::Developer:
            return "Developer";

        case ScreenID::Logbook:
            return "Bitacora";

        case ScreenID::Oracle:
            return "Oraculo";

        case ScreenID::Compass:
            return "Brujula";

        case ScreenID::Calendar:
            return "Calendario";

        case ScreenID::AI:
            return "Nauty Soul";

        default:
            return "Unknown";
    }
}

ScreenManager NQScreen;
