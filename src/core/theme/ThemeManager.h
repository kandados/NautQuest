#pragma once

#include <lvgl.h>

class ThemeManager
{
public:
    void begin();

    lv_color_t background() const;
    lv_color_t text() const;

    lv_color_t primaryButton() const;
    lv_color_t settingsButton() const;
    lv_color_t backButton() const;
};

extern ThemeManager NQTheme;