#include <Arduino.h>

#include "core/BootManager.h"
#include "storage/AssetManager.h"

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("========================================");
    Serial.println("            NautQuest OS");
    Serial.println("========================================");
    Serial.println("Sprint 017.5 - Recursos dinamicos SD + PSRAM");
    Serial.println("----------------------------------------");

    /*
     * AssetManager debe inicializarse antes que BootManager.
     *
     * NQAssets.begin() monta la microSD y deja disponible
     * el sistema de recursos.
     *
     * Más adelante, NQDisplay.begin() registrará la unidad S:
     * en LVGL para poder cargar archivos mediante rutas como:
     *
     * S:/ui/home/nauty_home.bin
     */
    const bool assetsReady = NQAssets.begin();

    if (assetsReady)
    {
        Serial.println(
            "[Main] AssetManager listo y microSD montada"
        );
    }
    else
    {
        Serial.println(
            "[Main] ERROR: AssetManager no pudo montar la microSD"
        );
    }

    Serial.println("----------------------------------------");

    /*
     * BootManager inicializa la pantalla y controla toda
     * la secuencia de arranque de NautQuest.
     *
     * No llamamos directamente a showHomeScreen() aquí porque
     * esa transición debe pertenecer al BootManager.
     */
    NQBoot.begin();
}

void loop()
{
    /*
     * BootManager actualiza LVGL y controla el avance entre
     * las diferentes pantallas del arranque.
     */
    NQBoot.update();

    delay(5);
}