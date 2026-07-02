#include "MissionStep.h"

MissionStep::MissionStep()
{
}

MissionStep::MissionStep(const char* title, bool completed)
{
    stepTitle = title;
    completedState = completed;
}

const char* MissionStep::title() const
{
    return stepTitle;
}

bool MissionStep::isCompleted() const
{
    return completedState;
}

void MissionStep::markCompleted()
{
    completedState = true;
}

void MissionStep::markPending()
{
    completedState = false;
}