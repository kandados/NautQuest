#pragma once

class NautQuestDisplay {
public:
    void begin();

    void showMascotBootScreen();
    void showLogoBootScreen();
    void showAdventureBootScreen();
    void showHomeScreen();

    void update();
};

extern NautQuestDisplay NQDisplay;