#include "NotificationManager.h"

#include <Arduino.h>

void NotificationManager::begin()
{
    visible = false;
    titleText = "";
    messageText = "";
    type = NotificationType::Info;
    startTime = 0;

    Serial.println("[NotificationManager] Inicializado");
}

void NotificationManager::update()
{
    if (!visible)
    {
        return;
    }

    unsigned long now = millis();

    if (now - startTime >= duration)
    {
        visible = false;
        Serial.println("[NotificationManager] Notificacion finalizada");
    }
}

void NotificationManager::show(NotificationType notificationType, const char* title, const char* message)
{
    type = notificationType;
    titleText = title;
    messageText = message;
    visible = true;
    startTime = millis();

    Serial.print("[NotificationManager] Nueva notificacion: ");
    Serial.println(titleText);

    Serial.print("[NotificationManager] Mensaje: ");
    Serial.println(messageText);
}

bool NotificationManager::isVisible() const
{
    return visible;
}

const char* NotificationManager::currentTitle() const
{
    return titleText;
}

const char* NotificationManager::currentMessage() const
{
    return messageText;
}

NotificationType NotificationManager::currentType() const
{
    return type;
}

NotificationManager NQNotification;