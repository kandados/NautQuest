#include "MissionDetailScreen.h"

#include <Arduino.h>
#include <lvgl.h>

#include "core/MissionManager.h"
#include "core/NavigationManager.h"
#include "core/theme/ThemeManager.h"

static int testStepIndex = 2;

static void completeStepButtonEventHandler(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.print("[MissionDetailScreen] Completar paso temporal: ");
        Serial.println(testStepIndex);

        NQMission.completeStep(testStepIndex);

        testStepIndex++;

        if (testStepIndex >= NQMission.activeMission().stepCount())
        {
            testStepIndex = 2;
        }

        NQMissionDetail.show();
    }
}

static void backButtonEventHandler(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[MissionDetailScreen] Volver pulsado");
        NQNavigation.goBack();
    }
}

void MissionDetailScreen::show()
{
    Mission &mission = NQMission.activeMission();

    lv_obj_clean(lv_scr_act());

    lv_color_t accentBlue = lv_palette_main(LV_PALETTE_BLUE);
    lv_color_t okGreen = lv_palette_main(LV_PALETTE_GREEN);
    lv_color_t pendingGrey = lv_palette_lighten(LV_PALETTE_GREY, 2);

    lv_obj_set_style_bg_color(lv_scr_act(), NQTheme.background(), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

    lv_obj_t *header = lv_label_create(lv_scr_act());
    lv_label_set_text(header, "OBJETIVOS");
    lv_obj_set_style_text_color(header, NQTheme.text(), 0);
    lv_obj_set_style_text_font(header, &lv_font_montserrat_34, 0);
    lv_obj_align(header, LV_ALIGN_CENTER, 0, -175);

    int y = -110;

    for (int i = 0; i < mission.stepCount(); i++)
    {
        MissionStep &step = mission.step(i);

        lv_obj_t *stepName = lv_label_create(lv_scr_act());
        lv_label_set_text(stepName, step.title());
        lv_obj_set_width(stepName, 285);
        lv_obj_set_style_text_color(stepName, NQTheme.text(), 0);
        lv_obj_set_style_text_font(stepName, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_align(stepName, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_align(stepName, LV_ALIGN_CENTER, -25, y);

        lv_obj_t *stepState = lv_label_create(lv_scr_act());
        lv_label_set_text(stepState, step.isCompleted() ? "OK" : "--");
        lv_obj_set_style_text_color(stepState, step.isCompleted() ? okGreen : pendingGrey, 0);
        lv_obj_set_style_text_font(stepState, &lv_font_montserrat_24, 0);
        lv_obj_align(stepState, LV_ALIGN_CENTER, 150, y - 2);

        y += 36;
    }

    char rewardText[32];
    snprintf(rewardText, sizeof(rewardText), "RECOMPENSA  %d EXP", mission.experienceReward());

    lv_obj_t *reward = lv_label_create(lv_scr_act());
    lv_label_set_text(reward, rewardText);
    lv_obj_set_style_text_color(reward, accentBlue, 0);
    lv_obj_set_style_text_font(reward, &lv_font_montserrat_20, 0);
    lv_obj_align(reward, LV_ALIGN_CENTER, 0, 70);

    lv_obj_t *level = lv_label_create(lv_scr_act());
    lv_label_set_text(level, "Completado");
    lv_obj_set_style_text_color(level, accentBlue, 0);
    lv_obj_set_style_text_font(level, &lv_font_montserrat_16, 0);
    lv_obj_align(level, LV_ALIGN_CENTER, -50, 100);

    lv_obj_t *progressBar = lv_bar_create(lv_scr_act());
    lv_obj_set_size(progressBar, 250, 14);
    lv_obj_align(progressBar, LV_ALIGN_CENTER, -35, 120);
    lv_bar_set_range(progressBar, 0, 100);
    lv_bar_set_value(progressBar, mission.progress(), LV_ANIM_OFF);

    char progressText[24];
    snprintf(progressText, sizeof(progressText), "%d%%", mission.progress());

    lv_obj_t *progressLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(progressLabel, progressText);
    lv_obj_set_style_text_color(progressLabel, accentBlue, 0);
    lv_obj_set_style_text_font(progressLabel, &lv_font_montserrat_18, 0);
    lv_obj_align(progressLabel, LV_ALIGN_CENTER, 145, 117);

    lv_obj_t *completeButton = lv_btn_create(lv_scr_act());
    lv_obj_set_size(completeButton, 155, 38);
    lv_obj_align(completeButton, LV_ALIGN_CENTER, -82, 170);
    lv_obj_add_event_cb(completeButton, completeStepButtonEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *completeLabel = lv_label_create(completeButton);
    lv_label_set_text(completeLabel, "Completar");
    lv_obj_set_style_text_font(completeLabel, &lv_font_montserrat_16, 0);
    lv_obj_center(completeLabel);
   
    lv_obj_t *backButton = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(backButton, NQTheme.backButton(), 0);
    lv_obj_set_size(backButton, 155, 38);
    lv_obj_align(backButton, LV_ALIGN_CENTER, 82, 170);
    lv_obj_add_event_cb(backButton, backButtonEventHandler, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *backLabel = lv_label_create(backButton);
    lv_label_set_text(backLabel, "Volver");
    lv_obj_set_style_text_font(backLabel, &lv_font_montserrat_16, 0);
    lv_obj_center(backLabel);

    lv_timer_handler();
}

MissionDetailScreen NQMissionDetail;
