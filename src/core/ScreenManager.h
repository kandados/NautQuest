#pragma once

enum class ScreenID
{
    Boot,
    Desktop,

    Settings,
    SettingsDisplay,
    SettingsSound,
    
    Missions,
    MissionDetail,
    Logbook,
    Oracle,
    Developer,
    Compass,
    Calendar,
    AI
};

class ScreenManager
{
public:
    void show(ScreenID screen);

    ScreenID currentScreen() const;
    ScreenID previousScreen() const;

    const char *currentScreenName() const;
    const char *previousScreenName() const;

    const char *nameOf(ScreenID screen) const;

private:
    ScreenID activeScreen = ScreenID::Boot;
    ScreenID lastScreen = ScreenID::Boot;

    const char *screenName(ScreenID screen) const;
};

extern ScreenManager NQScreen;