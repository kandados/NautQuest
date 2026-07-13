#pragma once

enum class CompanionMood
{
    Idle,
    Greeting,
    Happy,
    Thinking,
    Sleeping,
    Celebrating,
    Sad,
    Error
};

class CompanionManager
{
public:
    void begin();
    void update();

    void setMood(CompanionMood newMood);
    CompanionMood currentMood() const;

    const char* moodName() const;
    const char* message() const;

private:
    CompanionMood mood = CompanionMood::Idle;
    unsigned long moodStartTime = 0;
};

extern CompanionManager NQCompanion;