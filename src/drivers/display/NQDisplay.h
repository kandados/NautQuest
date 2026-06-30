#pragma once

class NautQuestDisplay {
public:
    void begin();

    void showMascotBootScreen();
    void showLogoBootScreen();
    void showAdventureBootScreen();

    void update();
};

extern NautQuestDisplay NQDisplay;