#pragma once

#include "MissionStep.h"

class Mission
{
public:
    Mission();

    const char* title() const;
    const char* description() const;
    const char* currentObjective() const;

    int progress() const;
    bool isCompleted() const;
    int experienceReward() const;

    int stepCount() const;
    MissionStep& step(int index);

private:
    static const int MAX_STEPS = 5;

    const char* missionTitle = "Construir NautQuest";
    const char* missionDescription = "Crear el primer prototipo funcional del sistema.";
    const char* objective = "Diseñar el nucleo de misiones.";

    int rewardExp = 250;

    MissionStep steps[MAX_STEPS] = {
        MissionStep("Crear arranque", true),
        MissionStep("Crear navegacion", true),
        MissionStep("Crear sistema de misiones", false),
        MissionStep("Integrar Bitacora", false),
        MissionStep("Integrar Oraculo", false)
    };
};