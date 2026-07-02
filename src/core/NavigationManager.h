#pragma once

#include "core/ScreenManager.h"

class NavigationManager
{
public:
    void begin();

    void goTo(ScreenID screen);
    void goBack();

    bool canGoBack() const;

private:
    static const int MAX_HISTORY = 10;

    ScreenID history[MAX_HISTORY];
    int historyCount = 0;
};

extern NavigationManager NQNavigation;