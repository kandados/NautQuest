#include "ThemeManager.h"

#include <Arduino.h>

void ThemeManager::begin()
{
    Serial.println("[ThemeManager] Tema visual inicializado");
}

lv_color_t ThemeManager::background() const
{
    return lv_color_black();
}

lv_color_t ThemeManager::text() const
{
    return lv_color_white();
}

lv_color_t ThemeManager::primaryButton() const
{
    return lv_palette_main(LV_PALETTE_BLUE);
}

lv_color_t ThemeManager::settingsButton() const
{
    return lv_palette_darken(LV_PALETTE_GREY, 2);
}

lv_color_t ThemeManager::backButton() const
{
    return lv_palette_main(LV_PALETTE_ORANGE);
}

ThemeManager NQTheme;