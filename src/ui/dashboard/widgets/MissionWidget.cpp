#include "MissionWidget.h"

#include "core/MissionManager.h"
#include "core/theme/ThemeManager.h"

MissionWidget NQMissionWidget;

lv_obj_t* MissionWidget::create(lv_obj_t* parent)
{
    Mission& mission = NQMission.activeMission();

    lv_obj_t* container = lv_obj_create(parent);

    lv_obj_set_size(container, 370, 105);
    lv_obj_set_style_radius(container, 18, 0);
    lv_obj_set_style_bg_color(container, lv_color_hex(0x111827), 0);
    lv_obj_set_style_bg_opa(container, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(container, 1, 0);
    lv_obj_set_style_border_color(container, lv_color_hex(0x2563EB), 0);
    lv_obj_set_style_pad_all(container, 12, 0);

    lv_obj_t* label = lv_label_create(container);
    lv_label_set_text(label, "MISION ACTIVA");
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t* title = lv_label_create(container);
    lv_label_set_text(title, mission.title());
    lv_obj_set_width(title, 340);
    lv_label_set_long_mode(title, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_color(title, NQTheme.text(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 24);

    lv_obj_t* objective = lv_label_create(container);
    lv_label_set_text(objective, mission.currentObjective());
    lv_obj_set_width(objective, 340);
    lv_label_set_long_mode(objective, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_color(objective, lv_color_hex(0xCBD5E1), 0);
    lv_obj_set_style_text_font(objective, &lv_font_montserrat_14, 0);
    lv_obj_align(objective, LV_ALIGN_TOP_LEFT, 0, 56);

    return container;
}