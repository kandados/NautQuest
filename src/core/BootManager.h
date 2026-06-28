#pragma once

class BootManager {
public:
    void begin();
    void update();

private:
    bool bootCompleted = false;
    unsigned long bootStartTime = 0;
};

extern BootManager NQBoot;