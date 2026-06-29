#pragma once

enum class ScreenID {
    Desktop
};

class ScreenManager {
public:
    void show(ScreenID screen);
};

extern ScreenManager NQScreen;