#pragma once

enum class NotificationType
{
    Info,
    MissionCompleted,
    LevelUp,
    Warning,
    Error
};

class NotificationManager
{
public:
    void begin();
    void update();

    void show(NotificationType type, const char* title, const char* message);

    bool isVisible() const;
    const char* currentTitle() const;
    const char* currentMessage() const;
    NotificationType currentType() const;

private:
    bool visible = false;

    const char* titleText = "";
    const char* messageText = "";
    NotificationType type = NotificationType::Info;

    unsigned long startTime = 0;
    unsigned long duration = 2500;
};

extern NotificationManager NQNotification;