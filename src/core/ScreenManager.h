#pragma once

enum class ScreenID {
    Boot,
    Desktop,
    Settings,
    Compass,
    Calendar,
    AI
};

class ScreenManager {
public:
    void show(ScreenID screen);
    ScreenID currentScreen() const;

private:
    ScreenID activeScreen = ScreenID::Boot;
};

extern ScreenManager NQScreen;