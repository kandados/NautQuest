#pragma once

#include "core/models/Mission.h"

class MissionManager
{
public:
    void begin();
    void update();

    Mission& activeMission();

    const char* currentMission() const;

    bool rewardAlreadyClaimed() const;
    void markRewardClaimed();

private:
    Mission currentActiveMission;
    bool rewardClaimed = false;
};

extern MissionManager NQMission;