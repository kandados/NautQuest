#include <Arduino.h>

#include "core/BootManager.h"
#include "nauty/NautyBehavior.h"
#include "storage/AssetManager.h"
#include "ui/companion/NautyAnimator.h"
#include "ui/companion/NautyVideoPlayer.h"

NautyBehavior nautyBehavior;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("========================================");
    Serial.println("            NautQuest OS");
    Serial.println("========================================");
    Serial.println("Sprint 019.1 - Estados conectados a NautyFace");
    Serial.println("----------------------------------------");

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

    NQBoot.begin();
    nautyBehavior.begin();

    Serial.println("----------------------------------------");
    Serial.println("[Main] NautyBehavior listo");
    Serial.println("----------------------------------------");
}

void loop()
{
    NQBoot.update();
    nautyBehavior.update();
    NQNautyAnimator.update();
    NQNautyVideo.update();

    delay(NQNautyVideo.isPlaying() ? 1 : 5);
}
