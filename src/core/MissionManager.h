#pragma once

class MissionManager
{
public:
    void begin();
    void update();

    const char* currentMission() const;

private:
    const char* activeMission = "La aventura comienza";
};

extern MissionManager NQMission;