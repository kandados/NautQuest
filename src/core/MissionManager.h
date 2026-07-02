#pragma once

#include "core/models/Mission.h"

class MissionManager
{
public:
    void begin();
    void update();

    Mission& activeMission();

    const char* currentMission() const;

private:
    Mission currentActiveMission;
};

extern MissionManager NQMission;