#include "MissionManager.h"
#include "ExperienceManager.h"

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

void MissionManager::completeStep(int index)
{
    if (index < 0 || index >= currentActiveMission.stepCount())
    {
        Serial.println("[MissionManager] Paso fuera de rango");
        return;
    }

    MissionStep& currentStep = currentActiveMission.step(index);

    if (currentStep.isCompleted())
    {
        Serial.println("[MissionManager] El paso ya estaba completado");
        return;
    }

    currentStep.markCompleted();

    Serial.print("[MissionManager] Paso completado: ");
    Serial.println(currentStep.title());

    Serial.print("[MissionManager] Nuevo progreso: ");
    Serial.print(currentActiveMission.progress());
    Serial.println("%");

    if (currentActiveMission.isCompleted() && !rewardAlreadyClaimed())
    {
        Serial.println("[MissionManager] ¡Mision completada!");

        NQExperience.addXP(currentActiveMission.experienceReward());

        markRewardClaimed();
    }
}

bool MissionManager::rewardAlreadyClaimed() const
{
    return rewardClaimed;
}

void MissionManager::markRewardClaimed()
{
    rewardClaimed = true;

    Serial.println("[MissionManager] Recompensa marcada como entregada");
}

MissionManager NQMission;