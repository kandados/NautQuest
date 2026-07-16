#pragma once

#include <lvgl.h>

#include "ui/common/BaseScreen.h"

class SoundSettingsScreen : public BaseScreen
{
protected:
    const char *screenTitle() const override;
    void buildContent(lv_obj_t *content) override;

private:
    static void volumeEventHandler(lv_event_t *event);
    static void switchEventHandler(lv_event_t *event);
    static void sensitivityEventHandler(lv_event_t *event);
    static void speakerTestEventHandler(lv_event_t *event);

    lv_obj_t *createSectionTitle(
        lv_obj_t *parent,
        const char *text
    );

    lv_obj_t *createSwitchCard(
        lv_obj_t *parent,
        const char *icon,
        const char *title,
        const char *description,
        const char *settingId,
        bool enabled
    );

    lv_obj_t *createSensitivityButton(
        lv_obj_t *parent,
        const char *text,
        int value
    );

    void updateSensitivitySelection(int value);

    lv_obj_t *volumeValueLabel = nullptr;

    lv_obj_t *lowSensitivityButton = nullptr;
    lv_obj_t *mediumSensitivityButton = nullptr;
    lv_obj_t *highSensitivityButton = nullptr;

    int selectedSensitivity = 1;
};

extern SoundSettingsScreen NQSoundSettings;