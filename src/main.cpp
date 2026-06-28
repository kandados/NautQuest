#include <Arduino.h>
#include "core/BootManager.h"

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("========================================");
    Serial.println("            NautQuest OS");
    Serial.println("========================================");
    Serial.println("Sprint 004 - Boot Manager");

    NQBoot.begin();
}

void loop() {
    NQBoot.update();
    delay(5);
}