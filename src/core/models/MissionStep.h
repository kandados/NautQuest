#pragma once

class MissionStep
{
public:
    MissionStep();
    MissionStep(const char* title, bool completed);

    const char* title() const;
    bool isCompleted() const;

    void markCompleted();
    void markPending();

private:
    const char* stepTitle = "";
    bool completedState = false;
};