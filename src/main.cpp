#include <Arduino.h>
#include "drivers/display/NQDisplay.h"

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("========================================");
    Serial.println("            NautQuest OS");
    Serial.println("========================================");
    Serial.println("Sprint 002 - AMOLED Boot Screen");

    NQDisplay.begin();
    NQDisplay.showBootScreen();

    Serial.println("[ OK ] NautQuest display boot screen loaded");
}

void loop() {
    NQDisplay.update();
    delay(5);
}