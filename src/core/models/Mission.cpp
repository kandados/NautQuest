#include "Mission.h"

Mission::Mission()
{
}

const char* Mission::title() const
{
    return missionTitle;
}

const char* Mission::description() const
{
    return missionDescription;
}

const char* Mission::currentObjective() const
{
    return objective;
}

int Mission::progress() const
{
    int completedSteps = 0;

    for (int i = 0; i < MAX_STEPS; i++)
    {
        if (steps[i].isCompleted())
        {
            completedSteps++;
        }
    }

    return (completedSteps * 100) / MAX_STEPS;
}

bool Mission::isCompleted() const
{
    return progress() >= 100;
}

int Mission::experienceReward() const
{
    return rewardExp;
}

int Mission::stepCount() const
{
    return MAX_STEPS;
}

MissionStep& Mission::step(int index)
{
    return steps[index];
}