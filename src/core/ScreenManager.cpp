#include "ScreenManager.h"

#include <Arduino.h>

#include "ui/desktop/DesktopScreen.h"

void ScreenManager::show(ScreenID screen)
{
    switch (screen)
    {
        case ScreenID::Desktop:
            Serial.println("[ScreenManager] Showing DesktopScreen");
            NQDesktop.show();
            break;
    }
}

ScreenManager NQScreen;