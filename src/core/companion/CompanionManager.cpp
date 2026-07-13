#include "CompanionManager.h"

#include <Arduino.h>

CompanionManager NQCompanion;

void CompanionManager::begin()
{
    mood = CompanionMood::Greeting;
    moodStartTime = millis();

    Serial.println("[CompanionManager] Inicializado");
    Serial.println("[CompanionManager] Estado: Greeting");
}

void CompanionManager::update()
{
    // De momento no hay lógica automática.
    // La añadiremos en el Sprint 018.
}

void CompanionManager::setMood(CompanionMood newMood)
{
    if (mood == newMood)
    {
        return;
    }

    mood = newMood;
    moodStartTime = millis();

    Serial.print("[CompanionManager] Nuevo estado: ");
    Serial.println(moodName());
}

CompanionMood CompanionManager::currentMood() const
{
    return mood;
}

const char* CompanionManager::moodName() const
{
    switch (mood)
    {
        case CompanionMood::Idle:
            return "Idle";

        case CompanionMood::Greeting:
            return "Greeting";

        case CompanionMood::Happy:
            return "Happy";

        case CompanionMood::Thinking:
            return "Thinking";

        case CompanionMood::Sleeping:
            return "Sleeping";

        case CompanionMood::Celebrating:
            return "Celebrating";

        case CompanionMood::Sad:
            return "Sad";

        case CompanionMood::Error:
            return "Error";

        default:
            return "Unknown";
    }
}

const char* CompanionManager::message() const
{
    switch (mood)
    {
        case CompanionMood::Greeting:
            return "Buenos dias, aventurero.";

        case CompanionMood::Idle:
            return "Estoy listo para una nueva aventura.";

        case CompanionMood::Happy:
            return "Buen trabajo.";

        case CompanionMood::Thinking:
            return "Estoy pensando...";

        case CompanionMood::Sleeping:
            return "Zzz...";

        case CompanionMood::Celebrating:
            return "¡Mision completada!";

        case CompanionMood::Sad:
            return "Podemos hacerlo mejor.";

        case CompanionMood::Error:
            return "Algo no ha salido bien.";

        default:
            return "";
    }
}