#include "DashboardScreen.h"

#include <Arduino.h>
#include <lvgl.h>


#include "core/NavigationManager.h"
#include "core/ScreenManager.h"
#include "storage/AssetManager.h"

DashboardScreen NQDashboard;

static lv_obj_t *tileview = nullptr;

/*
 * Coordenadas de las cubiertas dentro del TileView.
 *
 * Home:         (0, 0)
 * Aventura:     (0, 1)
 * Exploracion:  (1, 1)
 * Companero:    (2, 1)
 * Herramientas: (3, 1)
 */
static constexpr uint8_t HOME_COLUMN = 0;
static constexpr uint8_t HOME_ROW = 0;

static constexpr uint8_t ADVENTURE_COLUMN = 0;
static constexpr uint8_t EXPLORATION_COLUMN = 1;
static constexpr uint8_t COMPANION_COLUMN = 2;
static constexpr uint8_t TOOLS_COLUMN = 3;
static constexpr uint8_t LAUNCHER_ROW = 1;

/*
 * Recursos gráficos del Dashboard almacenados en la microSD.
 */
static constexpr const char *HOME_IMAGE_PATH =
    "S:/ui/home/nauty_home.bin";

static constexpr const char *ADVENTURE_IMAGE_PATH =
    "S:/ui/launcher/adventure.bin";

static constexpr const char *EXPLORATION_IMAGE_PATH =
    "S:/ui/launcher/exploration.bin";

static constexpr const char *COMPANION_IMAGE_PATH =
    "S:/ui/launcher/companion.bin";

static constexpr const char *TOOLS_IMAGE_PATH =
    "S:/ui/launcher/tools.bin";

/*
 * Navegacion existente.
 */

static void goMissions(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Abriendo Misiones");
        NQNavigation.goTo(ScreenID::Missions);
    }
}

static void goLogbook(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Abriendo Bitacora");
        NQNavigation.goTo(ScreenID::Logbook);
    }
}

static void goOracle(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Abriendo Oraculo");
        NQNavigation.goTo(ScreenID::Oracle);
    }
}

/*
 * Cubierta Exploracion.
 */

static void goCompass(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Abriendo Brujula");
        NQNavigation.goTo(ScreenID::Compass);
    }
}

static void goExplorer(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Explorador seleccionado");
    }
}

static void goCalendar(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Abriendo Calendario");
        NQNavigation.goTo(ScreenID::Calendar);
    }
}

/*
 * Cubierta Companero.
 */

static void goNauty(lv_event_t *event)
{
    if (
        lv_event_get_code(event) == LV_EVENT_CLICKED &&
        tileview != nullptr
    )
    {
        Serial.println("[Dashboard] Volviendo a Nauty");

        lv_obj_set_tile_id(
            tileview,
            HOME_COLUMN,
            HOME_ROW,
            LV_ANIM_ON
        );
    }
}

static void goAchievements(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Logros seleccionado");
    }
}

static void goProgress(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Progreso seleccionado");
    }
}

/*
 * Cubierta Herramientas.
 */

static void goInventory(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Inventario seleccionado");
    }
}

static void goCamera(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println(
            "[Dashboard] Camara seleccionada - funcion futura"
        );
    }
}

static void goSettings(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        Serial.println("[Dashboard] Abriendo Ajustes");
        NQNavigation.goTo(ScreenID::Settings);
    }
}

/*
 * Configuracion visual comun.
 */

static void configureTile(lv_obj_t *tile)
{
    lv_obj_set_style_bg_color(
        tile,
        lv_color_black(),
        0
    );

    lv_obj_set_style_bg_opa(
        tile,
        LV_OPA_COVER,
        0
    );

    lv_obj_set_style_pad_all(
        tile,
        0,
        0
    );

    lv_obj_set_style_border_width(
        tile,
        0,
        0
    );

    lv_obj_set_scrollbar_mode(
        tile,
        LV_SCROLLBAR_MODE_OFF
    );
}

/*
 * Carga la imagen una sola vez desde la microSD a PSRAM.
 * Durante los gestos LVGL dibuja desde memoria rápida.
 */
static lv_obj_t *createImageFromCache(
    lv_obj_t *parent,
    const char *path
)
{
    const lv_img_dsc_t *source =
        NQAssets.loadImageToPSRAM(path);

    if (source == nullptr)
    {
        Serial.print(
            "[Dashboard] ERROR cargando imagen: "
        );
        Serial.println(path);

        return nullptr;
    }

    lv_obj_t *image = lv_img_create(parent);

    lv_img_set_src(
        image,
        source
    );

    lv_obj_center(image);

    lv_obj_clear_flag(
        image,
        LV_OBJ_FLAG_CLICKABLE
    );

    lv_obj_clear_flag(
        image,
        LV_OBJ_FLAG_SCROLLABLE
    );

    return image;
}

static lv_obj_t *createInvisibleTouchArea(
    lv_obj_t *parent,
    lv_coord_t width,
    lv_coord_t height,
    lv_coord_t x,
    lv_coord_t y,
    lv_event_cb_t callback
)
{
    lv_obj_t *touchArea = lv_btn_create(parent);

    lv_obj_set_size(
        touchArea,
        width,
        height
    );

    lv_obj_align(
        touchArea,
        LV_ALIGN_CENTER,
        x,
        y
    );

    lv_obj_set_style_bg_opa(
        touchArea,
        LV_OPA_TRANSP,
        LV_PART_MAIN | LV_STATE_DEFAULT
    );

    lv_obj_set_style_border_width(
        touchArea,
        0,
        LV_PART_MAIN | LV_STATE_DEFAULT
    );

    lv_obj_set_style_shadow_width(
        touchArea,
        0,
        LV_PART_MAIN | LV_STATE_DEFAULT
    );

    lv_obj_set_style_outline_width(
        touchArea,
        0,
        LV_PART_MAIN | LV_STATE_DEFAULT
    );

    lv_obj_set_style_bg_opa(
        touchArea,
        LV_OPA_TRANSP,
        LV_PART_MAIN | LV_STATE_PRESSED
    );

    lv_obj_set_style_border_width(
        touchArea,
        0,
        LV_PART_MAIN | LV_STATE_PRESSED
    );

    lv_obj_set_style_shadow_width(
        touchArea,
        0,
        LV_PART_MAIN | LV_STATE_PRESSED
    );

    lv_obj_set_style_outline_width(
        touchArea,
        0,
        LV_PART_MAIN | LV_STATE_PRESSED
    );

    lv_obj_set_scrollbar_mode(
        touchArea,
        LV_SCROLLBAR_MODE_OFF
    );

    lv_obj_clear_flag(
        touchArea,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_add_flag(
        touchArea,
        LV_OBJ_FLAG_EVENT_BUBBLE
    );

    lv_obj_add_event_cb(
        touchArea,
        callback,
        LV_EVENT_CLICKED,
        nullptr
    );

    return touchArea;
}

static void createThreeTouchAreas(
    lv_obj_t *tile,
    lv_event_cb_t leftCallback,
    lv_event_cb_t centerCallback,
    lv_event_cb_t rightCallback
)
{
    createInvisibleTouchArea(
        tile,
        126,
        205,
        -132,
        25,
        leftCallback
    );

    createInvisibleTouchArea(
        tile,
        126,
        205,
        0,
        25,
        centerCallback
    );

    createInvisibleTouchArea(
        tile,
        126,
        205,
        132,
        25,
        rightCallback
    );
}

/*
 * Construccion de pantallas.
 */

static void buildHomeTile(lv_obj_t *tile)
{
    configureTile(tile);

    createImageFromCache(
        tile,
        HOME_IMAGE_PATH
    );
}

static void buildAdventureTile(lv_obj_t *tile)
{
    configureTile(tile);

    createImageFromCache(
        tile,
        ADVENTURE_IMAGE_PATH
    );

    createThreeTouchAreas(
        tile,
        goLogbook,
        goMissions,
        goOracle
    );
}

static void buildExplorationTile(lv_obj_t *tile)
{
    configureTile(tile);

    createImageFromCache(
        tile,
        EXPLORATION_IMAGE_PATH
    );

    createThreeTouchAreas(
        tile,
        goCompass,
        goExplorer,
        goCalendar
    );
}

static void buildCompanionTile(lv_obj_t *tile)
{
    configureTile(tile);

    createImageFromCache(
        tile,
        COMPANION_IMAGE_PATH
    );

    createThreeTouchAreas(
        tile,
        goNauty,
        goAchievements,
        goProgress
    );
}

static void buildToolsTile(lv_obj_t *tile)
{
    configureTile(tile);

    createImageFromCache(
        tile,
        TOOLS_IMAGE_PATH
    );

    createThreeTouchAreas(
        tile,
        goInventory,
        goCamera,
        goSettings
    );
}

void DashboardScreen::show()
{
    /*
     * Primero destruimos los objetos LVGL de la pantalla anterior.
     * Después ya es seguro liberar imágenes antiguas de la caché.
     */
    lv_obj_clean(lv_scr_act());
    NQAssets.clearImageCache();

    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_black(),
        0
    );

    lv_obj_set_style_bg_opa(
        lv_scr_act(),
        LV_OPA_COVER,
        0
    );

    tileview = lv_tileview_create(
        lv_scr_act()
    );

    lv_obj_set_size(
        tileview,
        466,
        466
    );

    lv_obj_center(tileview);

    lv_obj_set_style_anim_time(
        tileview,
        140,
        0
    );

    lv_obj_set_scroll_snap_x(
        tileview,
        LV_SCROLL_SNAP_CENTER
    );

    lv_obj_set_scroll_snap_y(
        tileview,
        LV_SCROLL_SNAP_CENTER
    );

    lv_obj_add_flag(
        tileview,
        LV_OBJ_FLAG_SCROLL_ONE
    );

    lv_obj_clear_flag(
        tileview,
        LV_OBJ_FLAG_SCROLL_MOMENTUM
    );

    lv_obj_clear_flag(
        tileview,
        LV_OBJ_FLAG_SCROLL_ELASTIC
    );

    lv_obj_set_style_pad_all(
        tileview,
        0,
        0
    );

    lv_obj_set_style_border_width(
        tileview,
        0,
        0
    );

    lv_obj_set_style_bg_color(
        tileview,
        lv_color_black(),
        0
    );

    lv_obj_set_style_bg_opa(
        tileview,
        LV_OPA_COVER,
        0
    );

    lv_obj_set_scrollbar_mode(
        tileview,
        LV_SCROLLBAR_MODE_OFF
    );

    lv_obj_t *homeTile = lv_tileview_add_tile(
        tileview,
        HOME_COLUMN,
        HOME_ROW,
        LV_DIR_BOTTOM
    );

    lv_obj_t *adventureTile = lv_tileview_add_tile(
        tileview,
        ADVENTURE_COLUMN,
        LAUNCHER_ROW,
        LV_DIR_TOP | LV_DIR_RIGHT
    );

    lv_obj_t *explorationTile = lv_tileview_add_tile(
        tileview,
        EXPLORATION_COLUMN,
        LAUNCHER_ROW,
        LV_DIR_LEFT | LV_DIR_RIGHT
    );

    lv_obj_t *companionTile = lv_tileview_add_tile(
        tileview,
        COMPANION_COLUMN,
        LAUNCHER_ROW,
        LV_DIR_LEFT | LV_DIR_RIGHT
    );

    lv_obj_t *toolsTile = lv_tileview_add_tile(
        tileview,
        TOOLS_COLUMN,
        LAUNCHER_ROW,
        LV_DIR_LEFT
    );

    buildHomeTile(homeTile);
    buildAdventureTile(adventureTile);
    buildExplorationTile(explorationTile);
    buildCompanionTile(companionTile);
    buildToolsTile(toolsTile);

    lv_obj_set_tile_id(
        tileview,
        HOME_COLUMN,
        HOME_ROW,
        LV_ANIM_OFF
    );

    lv_timer_handler();
}

void DashboardScreen::refresh()
{
    show();
}