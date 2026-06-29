#pragma once

enum class ScreenID
{
    Boot,
    Desktop,
    Settings,
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

    const char* currentScreenName() const;
    const char* previousScreenName() const;

private:
    ScreenID activeScreen = ScreenID::Boot;
    ScreenID lastScreen = ScreenID::Boot;

    const char* screenName(ScreenID screen) const;
};

extern ScreenManager NQScreen;