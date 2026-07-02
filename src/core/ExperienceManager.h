#pragma once

class ExperienceManager
{
public:
    void begin();

    int currentXP() const;
    int currentLevel() const;
    int xpForNextLevel() const;

    void addXP(int amount);

private:
    int xp = 0;
    int level = 1;

    void recalculateLevel();
};

extern ExperienceManager NQExperience;