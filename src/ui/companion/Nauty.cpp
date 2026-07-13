#include "Nauty.h"

#include "core/theme/ThemeManager.h"

Nauty NQNauty;

void Nauty::create(lv_obj_t *parent)
{
    root = lv_obj_create(parent);
    lv_obj_set_size(root, 190, 190);
    lv_obj_set_style_bg_opa(root, LV_OPA_0, 0);
    lv_obj_set_style_border_width(root, 0, 0);
    lv_obj_set_style_pad_all(root, 0, 0);
    lv_obj_set_scrollbar_mode(root, LV_SCROLLBAR_MODE_OFF);

    antenna = lv_obj_create(root);
    lv_obj_set_size(antenna, 14, 14);
    lv_obj_align(antenna, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_radius(antenna, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(antenna, lv_color_hex(0x22D3EE), 0);
    lv_obj_set_style_bg_opa(antenna, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(antenna, 0, 0);

    helmet = lv_obj_create(root);
    lv_obj_set_size(helmet, 150, 118);
    lv_obj_align(helmet, LV_ALIGN_TOP_MID, 0, 18);
    lv_obj_set_style_radius(helmet, 42, 0);
    lv_obj_set_style_bg_color(helmet, lv_color_hex(0xF8FAFC), 0);
    lv_obj_set_style_bg_opa(helmet, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(helmet, 3, 0);
    lv_obj_set_style_border_color(helmet, lv_color_hex(0x67E8F9), 0);
    lv_obj_set_scrollbar_mode(helmet, LV_SCROLLBAR_MODE_OFF);

    visor = lv_obj_create(helmet);
    lv_obj_set_size(visor, 112, 62);
    lv_obj_align(visor, LV_ALIGN_CENTER, 0, 4);
    lv_obj_set_style_radius(visor, 24, 0);
    lv_obj_set_style_bg_color(visor, lv_color_hex(0x020617), 0);
    lv_obj_set_style_bg_opa(visor, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(visor, 2, 0);
    lv_obj_set_style_border_color(visor, lv_color_hex(0x38BDF8), 0);
    lv_obj_set_scrollbar_mode(visor, LV_SCROLLBAR_MODE_OFF);

    leftEye = lv_obj_create(visor);
    lv_obj_set_size(leftEye, 18, 18);
    lv_obj_align(leftEye, LV_ALIGN_CENTER, -28, -6);
    lv_obj_set_style_radius(leftEye, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(leftEye, lv_color_hex(0x38BDF8), 0);
    lv_obj_set_style_bg_opa(leftEye, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(leftEye, 0, 0);

    rightEye = lv_obj_create(visor);
    lv_obj_set_size(rightEye, 18, 18);
    lv_obj_align(rightEye, LV_ALIGN_CENTER, 28, -6);
    lv_obj_set_style_radius(rightEye, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(rightEye, lv_color_hex(0x38BDF8), 0);
    lv_obj_set_style_bg_opa(rightEye, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(rightEye, 0, 0);

    mouth = lv_label_create(visor);
    lv_label_set_text(mouth, "⌣");
    lv_obj_set_style_text_color(mouth, lv_color_hex(0x38BDF8), 0);
    lv_obj_set_style_text_font(mouth, &lv_font_montserrat_24, 0);
    lv_obj_align(mouth, LV_ALIGN_CENTER, 0, 16);
}

void Nauty::idle()
{
    if (!leftEye || !rightEye || !mouth) return;

    lv_obj_set_size(leftEye, 18, 18);
    lv_obj_set_size(rightEye, 18, 18);
    lv_label_set_text(mouth, "⌣");

    lv_obj_align(leftEye, LV_ALIGN_CENTER, -28, -6);
    lv_obj_align(rightEye, LV_ALIGN_CENTER, 28, -6);
    lv_obj_align(mouth, LV_ALIGN_CENTER, 0, 16);
}

void Nauty::blink()
{
    if (!leftEye || !rightEye || !mouth) return;

    lv_obj_set_size(leftEye, 22, 4);
    lv_obj_set_size(rightEye, 22, 4);

    lv_obj_align(leftEye, LV_ALIGN_CENTER, -28, -6);
    lv_obj_align(rightEye, LV_ALIGN_CENTER, 28, -6);
}

void Nauty::happy()
{
    if (!leftEye || !rightEye || !mouth) return;

    lv_obj_set_size(leftEye, 24, 6);
    lv_obj_set_size(rightEye, 24, 6);
    lv_label_set_text(mouth, "⌣");

    lv_obj_align(leftEye, LV_ALIGN_CENTER, -28, -10);
    lv_obj_align(rightEye, LV_ALIGN_CENTER, 28, -10);
    lv_obj_align(mouth, LV_ALIGN_CENTER, 0, 16);
}

void Nauty::thinking()
{
    if (!leftEye || !rightEye || !mouth) return;

    lv_obj_set_size(leftEye, 12, 18);
    lv_obj_set_size(rightEye, 18, 18);
    lv_label_set_text(mouth, "—");

    lv_obj_align(leftEye, LV_ALIGN_CENTER, -28, -6);
    lv_obj_align(rightEye, LV_ALIGN_CENTER, 28, -6);
    lv_obj_align(mouth, LV_ALIGN_CENTER, 0, 16);
}

void Nauty::sleeping()
{
    if (!leftEye || !rightEye || !mouth) return;

    lv_obj_set_size(leftEye, 24, 4);
    lv_obj_set_size(rightEye, 24, 4);
    lv_label_set_text(mouth, "—");

    lv_obj_align(leftEye, LV_ALIGN_CENTER, -28, -6);
    lv_obj_align(rightEye, LV_ALIGN_CENTER, 28, -6);
    lv_obj_align(mouth, LV_ALIGN_CENTER, 0, 16);
}

void Nauty::surprised()
{
    if (!leftEye || !rightEye || !mouth) return;

    lv_obj_set_size(leftEye, 20, 20);
    lv_obj_set_size(rightEye, 20, 20);
    lv_label_set_text(mouth, "o");

    lv_obj_align(leftEye, LV_ALIGN_CENTER, -28, -8);
    lv_obj_align(rightEye, LV_ALIGN_CENTER, 28, -8);
    lv_obj_align(mouth, LV_ALIGN_CENTER, 0, 17);
}

void Nauty::sad()
{
    if (!leftEye || !rightEye || !mouth) return;

    lv_obj_set_size(leftEye, 22, 5);
    lv_obj_set_size(rightEye, 22, 5);
    lv_label_set_text(mouth, "⌢");

    lv_obj_align(leftEye, LV_ALIGN_CENTER, -28, -8);
    lv_obj_align(rightEye, LV_ALIGN_CENTER, 28, -8);
    lv_obj_align(mouth, LV_ALIGN_CENTER, 0, 18);
}

void Nauty::update()
{
    // Aquí añadiremos parpadeo, respiración y flotación.
}