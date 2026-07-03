#include "ExperienceManager.h"

#include <Arduino.h>

void ExperienceManager::begin()
{
    xp = 0;
    level = 1;

    Serial.println("[ExperienceManager] Inicializado");
    Serial.print("[ExperienceManager] Nivel actual: ");
    Serial.println(level);
    Serial.print("[ExperienceManager] XP actual: ");
    Serial.println(xp);
}

int ExperienceManager::currentXP() const
{
    return xp;
}

int ExperienceManager::currentLevel() const
{
    return level;
}

int ExperienceManager::xpForNextLevel() const
{
    return level * 100;
}

bool ExperienceManager::addXP(int amount)
{
    if (amount <= 0)
    {
        return false;
    }

    int previousLevel = level;

    xp += amount;

    Serial.print("[ExperienceManager] XP anadida: ");
    Serial.println(amount);

    recalculateLevel();

    return level != previousLevel;
}

void ExperienceManager::recalculateLevel()
{
    int newLevel = (xp / 100) + 1;

    if (newLevel != level)
    {
        level = newLevel;

        Serial.print("[ExperienceManager] Nuevo nivel: ");
        Serial.println(level);
    }
}

ExperienceManager NQExperience;