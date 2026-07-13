#include "StatusWidget.h"

#include "core/companion/CompanionManager.h"
#include "core/theme/ThemeManager.h"

StatusWidget NQStatusWidget;

lv_obj_t* StatusWidget::create(lv_obj_t* parent)
{
    lv_obj_t* container = lv_obj_create(parent);

    lv_obj_set_size(container, 360, 60);

    lv_obj_set_style_bg_opa(container, LV_OPA_0, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);

    lv_obj_t* label = lv_label_create(container);

    lv_label_set_text(label, NQCompanion.message());

    lv_obj_set_width(label, 340);

    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);

    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_set_style_text_font(label,
                               &lv_font_montserrat_18,
                               0);

    lv_obj_set_style_text_color(label,
                                NQTheme.text(),
                                0);

    lv_obj_center(label);

    return container;
}