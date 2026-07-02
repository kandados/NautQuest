#include "MissionManager.h"

#include <Arduino.h>

void MissionManager::begin()
{
    Serial.println("[MissionManager] Inicializando...");
    Serial.print("[MissionManager] Mision activa: ");
    Serial.println(currentActiveMission.title());

    Serial.print("[MissionManager] Objetivo actual: ");
    Serial.println(currentActiveMission.currentObjective());

    Serial.print("[MissionManager] Progreso: ");
    Serial.print(currentActiveMission.progress());
    Serial.println("%");
}

void MissionManager::update()
{
    // De momento no hace nada.
}

Mission& MissionManager::activeMission()
{
    return currentActiveMission;
}

const char* MissionManager::currentMission() const
{
    return currentActiveMission.title();
}

MissionManager NQMission;