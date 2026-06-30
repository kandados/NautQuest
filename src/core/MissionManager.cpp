#include "MissionManager.h"

#include <Arduino.h>

void MissionManager::begin()
{
    Serial.println("[MissionManager] Inicializando...");
    Serial.print("[MissionManager] Mision activa: ");
    Serial.println(activeMission);
}

void MissionManager::update()
{
    // De momento no hace nada.
}

const char* MissionManager::currentMission() const
{
    return activeMission;
}

MissionManager NQMission;