#pragma once

class BootManager
{
public:
    void begin();
    void update();

private:
    enum class BootPhase
    {
        Mascot,
        Logo,
        Adventure,
        Completed
    };

    BootPhase phase = BootPhase::Mascot;

    unsigned long phaseStartTime = 0;
};

extern BootManager NQBoot;