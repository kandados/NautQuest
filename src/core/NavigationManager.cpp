#include "core/NavigationManager.h"

#include <Arduino.h>

void NavigationManager::begin()
{
    historyCount = 0;
    Serial.println("[NavigationManager] Inicializado");
}

void NavigationManager::goTo(ScreenID screen)
{
    /*
     * Evita añadir al historial una navegación hacia
     * la pantalla que ya está activa.
     */
    if (screen == NQScreen.currentScreen())
    {
        return;
    }

    if (historyCount < MAX_HISTORY)
    {
        history[historyCount] = NQScreen.currentScreen();
        historyCount++;
    }

    Serial.print("[NavigationManager] Navegando a: ");
    Serial.println(NQScreen.nameOf(screen));

    NQScreen.show(screen);
}

void NavigationManager::goBack()
{
    if (!canGoBack())
    {
        Serial.println("[NavigationManager] No hay pantalla anterior");
        return;
    }

    historyCount--;
    ScreenID previous = history[historyCount];

    Serial.print("[NavigationManager] Volviendo a: ");
    Serial.println(NQScreen.nameOf(previous));

    NQScreen.show(previous);
}

bool NavigationManager::canGoBack() const
{
    return historyCount > 0;
}

NavigationManager NQNavigation;
