#pragma once

#include "ui/common/BaseScreen.h"

class DisplaySettingsScreen : public BaseScreen
{
protected:
    const char *screenTitle() const override;
    void buildContent(lv_obj_t *content) override;

private:
    static void brightnessEventHandler(lv_event_t *event);
    static void optionEventHandler(lv_event_t *event);

    lv_obj_t *createSectionTitle(
        lv_obj_t *parent,
        const char *text
    );

    lv_obj_t *createOptionCard(
        lv_obj_t *parent,
        const char *title,
        const char *value,
        const char *optionId
    );

    lv_obj_t *brightnessValueLabel = nullptr;
};

extern DisplaySettingsScreen NQDisplaySettings;