#pragma once

#include <lvgl.h>

/*
 * BaseScreen
 *
 * Base visual común para todas las pantallas interiores
 * de NautQuest.
 */
class BaseScreen
{
public:
    virtual ~BaseScreen() = default;

    void show();

protected:
    virtual const char *screenTitle() const = 0;
    virtual void buildContent(lv_obj_t *content) = 0;

    virtual bool hasBackButton() const;
    virtual bool hasBottomBar() const;
    virtual void onScreenReady();

    lv_obj_t *rootObject() const;
    lv_obj_t *headerObject() const;
    lv_obj_t *contentObject() const;
    lv_obj_t *bottomBarObject() const;

private:
    static void backButtonEventHandler(lv_event_t *event);

    void createRoot();
    void createHeader();
    void createContent();
    void createBottomBar();
    void createBackTouchOverlay();

    lv_obj_t *root = nullptr;
    lv_obj_t *header = nullptr;
    lv_obj_t *content = nullptr;
    lv_obj_t *bottomBar = nullptr;
};