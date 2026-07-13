#include "CompanionWidget.h"

#include "core/companion/CompanionManager.h"
#include "core/theme/ThemeManager.h"

CompanionWidget NQCompanionWidget;

lv_obj_t* CompanionWidget::create(lv_obj_t* parent)
{
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container, 210, 145);
    lv_obj_set_style_bg_color(container, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(container, LV_OPA_0, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);

    lv_obj_t* helmet = lv_obj_create(container);
    lv_obj_set_size(helmet, 130, 95);
    lv_obj_align(helmet, LV_ALIGN_CENTER, 0, -10);
    lv_obj_set_style_radius(helmet, 35, 0);
    lv_obj_set_style_bg_color(helmet, lv_color_hex(0xEAF6FF), 0);
    lv_obj_set_style_bg_opa(helmet, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(helmet, 3, 0);
    lv_obj_set_style_border_color(helmet, lv_color_hex(0x38BDF8), 0);

    lv_obj_t* visor = lv_obj_create(helmet);
    lv_obj_set_size(visor, 92, 48);
    lv_obj_align(visor, LV_ALIGN_CENTER, 0, 2);
    lv_obj_set_style_radius(visor, 18, 0);
    lv_obj_set_style_bg_color(visor, lv_color_hex(0x071827), 0);
    lv_obj_set_style_bg_opa(visor, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(visor, 2, 0);
    lv_obj_set_style_border_color(visor, lv_color_hex(0x60A5FA), 0);

    lv_obj_t* mood = lv_label_create(visor);
    lv_label_set_text(mood, NQCompanion.moodName());
    lv_obj_set_style_text_color(mood, lv_color_hex(0x7DD3FC), 0);
    lv_obj_set_style_text_font(mood, &lv_font_montserrat_14, 0);
    lv_obj_center(mood);

    lv_obj_t* name = lv_label_create(container);
    lv_label_set_text(name, "NAUTY");
    lv_obj_set_style_text_color(name, NQTheme.text(), 0);
    lv_obj_set_style_text_font(name, &lv_font_montserrat_20, 0);
    lv_obj_align(name, LV_ALIGN_CENTER, 0, 58);

    return container;
}