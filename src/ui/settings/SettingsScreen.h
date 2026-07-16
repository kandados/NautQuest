#pragma once

#include <lvgl.h>

#include "ui/common/BaseScreen.h"

class SettingsScreen : public BaseScreen
{
public:
    /*
     * Paleta visual de cada botón.
     *
     * Debe ser pública porque las paletas constantes
     * se crean fuera de la clase en SettingsScreen.cpp.
     */
    struct ButtonPalette
    {
        uint32_t border;
        uint32_t glow;
        uint32_t gradient;
        uint32_t icon;
    };

protected:
    const char *screenTitle() const override;
    void buildContent(lv_obj_t *content) override;

private:
    static void settingsButtonEventHandler(
        lv_event_t *event
    );

    lv_obj_t *createSettingsButton(
        lv_obj_t *parent,
        const char *icon,
        const char *title,
        const char *itemId,
        lv_coord_t x,
        lv_coord_t y,
        const ButtonPalette &palette
    );

    void createPageIndicators(
        lv_obj_t *parent
    );
};

extern SettingsScreen NQSettings;