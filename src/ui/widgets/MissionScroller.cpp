#include "MissionScroller.h"

#include <Arduino.h>

#include "core/theme/ThemeManager.h"

MissionScroller NQMissionScroller;

static int clampInt(int value, int minValue, int maxValue)
{
    if (value < minValue) return minValue;
    if (value > maxValue) return maxValue;
    return value;
}

void MissionScroller::create(lv_obj_t *parent)
{
    itemCount = 0;

    container = lv_obj_create(parent);
    lv_obj_set_size(container, 390, 250);
    lv_obj_align(container, LV_ALIGN_CENTER, 0, 15);

    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);

    lv_obj_set_scroll_dir(container, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);

    lv_obj_add_event_cb(container, scrollEvent, LV_EVENT_SCROLL, this);
}

void MissionScroller::clear()
{
    if (container != nullptr)
    {
        lv_obj_del(container);
        container = nullptr;
    }

    itemCount = 0;
}

void MissionScroller::addPlainItem(const char *text)
{
    addInternalItem(text, "", false);
}

void MissionScroller::addItem(const char *text, bool completed)
{
    addInternalItem(text, completed ? "OK" : "--", completed);
}

void MissionScroller::addInternalItem(const char *text, const char *state, bool completed)
{
    if (container == nullptr || itemCount >= MAX_ITEMS)
    {
        return;
    }

    int y = itemCount * 44;

    labels[itemCount] = lv_label_create(container);
    lv_label_set_text(labels[itemCount], text);
    lv_obj_set_width(labels[itemCount], 285);
    lv_obj_set_style_text_color(labels[itemCount], NQTheme.text(), 0);
    lv_obj_set_style_text_font(labels[itemCount], &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_align(labels[itemCount], LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(labels[itemCount], LV_ALIGN_TOP_LEFT, 18, y);

    states[itemCount] = lv_label_create(container);
    lv_label_set_text(states[itemCount], state);

    lv_obj_set_style_text_color(
        states[itemCount],
        completed ? lv_palette_main(LV_PALETTE_GREEN) : lv_palette_lighten(LV_PALETTE_GREY, 2),
        0
    );

    lv_obj_set_style_text_font(states[itemCount], &lv_font_montserrat_20, 0);
    lv_obj_align(states[itemCount], LV_ALIGN_TOP_RIGHT, -22, y - 2);

    itemCount++;
}

void MissionScroller::refresh()
{
    if (container == nullptr)
    {
        return;
    }

    lv_coord_t containerCenterY = lv_obj_get_y(container) + lv_obj_get_height(container) / 2;
    lv_coord_t scrollY = lv_obj_get_scroll_y(container);

    for (int i = 0; i < itemCount; i++)
    {
        lv_coord_t itemBaseY = i * 44;
        lv_coord_t itemCenterY = lv_obj_get_y(container) + itemBaseY - scrollY + 14;

        int distance = abs(itemCenterY - containerCenterY);
        int influence = clampInt(110 - distance, 0, 110);

        lv_opa_t opacity = static_cast<lv_opa_t>(clampInt(80 + influence, 80, 255));

        const lv_font_t *font = &lv_font_montserrat_18;

        if (influence > 85)
        {
            font = &lv_font_montserrat_26;
        }
        else if (influence > 55)
        {
            font = &lv_font_montserrat_22;
        }
        else if (influence > 25)
        {
            font = &lv_font_montserrat_20;
        }

        int horizontalOffset = influence / 7;

        lv_obj_set_style_text_font(labels[i], font, 0);
        lv_obj_set_style_text_opa(labels[i], opacity, 0);
        lv_obj_set_style_text_font(states[i], font, 0);
        lv_obj_set_style_text_opa(states[i], opacity, 0);

        lv_obj_align(labels[i], LV_ALIGN_TOP_LEFT, 18 + horizontalOffset, itemBaseY);
        lv_obj_align(states[i], LV_ALIGN_TOP_RIGHT, -22 - horizontalOffset, itemBaseY - 2);
    }
}

void MissionScroller::scrollEvent(lv_event_t *e)
{
    MissionScroller *scroller = static_cast<MissionScroller *>(lv_event_get_user_data(e));

    if (scroller != nullptr)
    {
        scroller->refresh();
    }
}