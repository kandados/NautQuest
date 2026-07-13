#include "XPWidget.h"

#include <Arduino.h>

#include "core/ExperienceManager.h"
#include "core/theme/ThemeManager.h"

XPWidget NQXPWidget;

lv_obj_t* XPWidget::create(lv_obj_t* parent)
{
    lv_obj_t* container = lv_obj_create(parent);

    lv_obj_set_size(container, 360, 58);
    lv_obj_set_style_bg_opa(container, LV_OPA_0, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);

    char levelText[32];
    snprintf(levelText, sizeof(levelText), "Nivel %d", NQExperience.currentLevel());

    lv_obj_t* levelLabel = lv_label_create(container);
    lv_label_set_text(levelLabel, levelText);
    lv_obj_set_style_text_color(levelLabel, NQTheme.text(), 0);
    lv_obj_set_style_text_font(levelLabel, &lv_font_montserrat_18, 0);
    lv_obj_align(levelLabel, LV_ALIGN_LEFT_MID, 0, -12);

    lv_obj_t* bar = lv_bar_create(container);
    lv_obj_set_size(bar, 220, 12);
    lv_obj_align(bar, LV_ALIGN_RIGHT_MID, 0, -10);
    lv_bar_set_range(bar, 0, NQExperience.xpForNextLevel());

    int xpInCurrentLevel = NQExperience.currentXP() % 100;
    lv_bar_set_value(bar, xpInCurrentLevel, LV_ANIM_OFF);

    char xpText[40];
    snprintf(
        xpText,
        sizeof(xpText),
        "%d XP",
        NQExperience.currentXP()
    );

    lv_obj_t* xpLabel = lv_label_create(container);
    lv_label_set_text(xpLabel, xpText);
    lv_obj_set_style_text_color(xpLabel, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_text_font(xpLabel, &lv_font_montserrat_14, 0);
    lv_obj_align(xpLabel, LV_ALIGN_CENTER, 0, 18);

    return container;
}