#include "NautyBehavior.h"
#include <esp_system.h>

namespace
{
    constexpr uint32_t MIN_IDLE_ACTION_DELAY_MS = 2500;
    constexpr uint32_t MAX_IDLE_ACTION_DELAY_MS = 6500;

    constexpr uint32_t BLINK_DURATION_MS = 180;
    constexpr uint32_t LOOK_DURATION_MS = 700;
}

void NautyBehavior::begin()
{
    randomSeed(
        static_cast<uint32_t>(micros()) ^
        static_cast<uint32_t>(esp_random())
    );

    currentState = NautyState::IDLE;
    previousState = NautyState::IDLE;
    currentPriority = NautyPriority::BACKGROUND;

    stateStartedAt = millis();
    stateDuration = 0;
    stateChanged = true;

    scheduleNextIdleAction(stateStartedAt);

    Serial.println("[NautyBehavior] Sistema de comportamiento iniciado");
    Serial.println("[NautyBehavior] Estado inicial: IDLE");
}

void NautyBehavior::update()
{
    const uint32_t now = millis();

    updateCurrentState(now);

    if (currentState == NautyState::IDLE)
    {
        updateIdleBehavior(now);
    }
}

bool NautyBehavior::requestState(
    NautyState state,
    uint32_t durationMs,
    NautyPriority priority
)
{
    /*
     * No permitimos que una expresión menos importante interrumpa
     * otra que ya se está mostrando.
     */
    if (
        currentState != NautyState::IDLE &&
        static_cast<uint8_t>(priority) <
            static_cast<uint8_t>(currentPriority)
    )
    {
        Serial.printf(
            "[NautyBehavior] Estado %s rechazado por prioridad\n",
            stateToString(state)
        );

        return false;
    }

    changeState(state, durationMs, priority);
    return true;
}

void NautyBehavior::returnToIdle()
{
    changeState(
        NautyState::IDLE,
        0,
        NautyPriority::BACKGROUND
    );

    scheduleNextIdleAction(millis());
}

NautyState NautyBehavior::getState() const
{
    return currentState;
}

NautyState NautyBehavior::getPreviousState() const
{
    return previousState;
}

NautyPriority NautyBehavior::getPriority() const
{
    return currentPriority;
}

uint32_t NautyBehavior::getStateElapsedTime() const
{
    return millis() - stateStartedAt;
}

uint32_t NautyBehavior::getStateRemainingTime() const
{
    if (stateDuration == 0)
    {
        return 0;
    }

    const uint32_t elapsed = getStateElapsedTime();

    if (elapsed >= stateDuration)
    {
        return 0;
    }

    return stateDuration - elapsed;
}

bool NautyBehavior::isIdle() const
{
    return currentState == NautyState::IDLE;
}

bool NautyBehavior::hasStateChanged()
{
    const bool changed = stateChanged;
    stateChanged = false;

    return changed;
}

const char* NautyBehavior::getStateName() const
{
    return stateToString(currentState);
}

void NautyBehavior::changeState(
    NautyState newState,
    uint32_t durationMs,
    NautyPriority priority
)
{
    if (newState == currentState)
    {
        /*
         * Aunque sea el mismo estado, reiniciamos su duración.
         * Esto permite prolongar una expresión ya activa.
         */
        stateStartedAt = millis();
        stateDuration = durationMs;
        currentPriority = priority;
        return;
    }

    previousState = currentState;
    currentState = newState;
    currentPriority = priority;

    stateStartedAt = millis();
    stateDuration = durationMs;
    stateChanged = true;

    Serial.printf(
        "[NautyBehavior] %s -> %s",
        stateToString(previousState),
        stateToString(currentState)
    );

    if (durationMs > 0)
    {
        Serial.printf(" durante %lu ms", durationMs);
    }

    Serial.println();
}

void NautyBehavior::updateCurrentState(uint32_t now)
{
    /*
     * Un estado con duración 0 permanece activo hasta que otro
     * componente lo cambie explícitamente.
     */
    if (stateDuration == 0)
    {
        return;
    }

    if ((now - stateStartedAt) >= stateDuration)
    {
        returnToIdle();
    }
}

void NautyBehavior::updateIdleBehavior(uint32_t now)
{
    if (now < nextIdleActionAt)
    {
        return;
    }

    /*
     * Distribución inicial de microcomportamientos:
     *
     * 0-59  -> parpadeo
     * 60-79 -> mirar a la izquierda
     * 80-99 -> mirar a la derecha
     *
     * El parpadeo ocurre con mayor frecuencia porque es el gesto
     * más natural y menos intrusivo.
     */
    const uint8_t action = static_cast<uint8_t>(random(0, 100));

    if (action < 60)
    {
        changeState(
            NautyState::BLINK,
            BLINK_DURATION_MS,
            NautyPriority::BACKGROUND
        );
    }
    else if (action < 80)
    {
        changeState(
            NautyState::LOOK_LEFT,
            LOOK_DURATION_MS,
            NautyPriority::BACKGROUND
        );
    }
    else
    {
        changeState(
            NautyState::LOOK_RIGHT,
            LOOK_DURATION_MS,
            NautyPriority::BACKGROUND
        );
    }
}

void NautyBehavior::scheduleNextIdleAction(uint32_t now)
{
    nextIdleActionAt =
        now +
        static_cast<uint32_t>(
            random(
                MIN_IDLE_ACTION_DELAY_MS,
                MAX_IDLE_ACTION_DELAY_MS + 1
            )
        );
}

const char* NautyBehavior::stateToString(NautyState state)
{
    switch (state)
    {
        case NautyState::IDLE:
            return "IDLE";

        case NautyState::BLINK:
            return "BLINK";

        case NautyState::LOOK_LEFT:
            return "LOOK_LEFT";

        case NautyState::LOOK_RIGHT:
            return "LOOK_RIGHT";

        case NautyState::HAPPY:
            return "HAPPY";

        case NautyState::SAD:
            return "SAD";

        case NautyState::ANGRY:
            return "ANGRY";

        case NautyState::CURIOUS:
            return "CURIOUS";

        case NautyState::UNCERTAIN:
            return "UNCERTAIN";

        case NautyState::TIRED:
            return "TIRED";

        case NautyState::SLEEPY:
            return "SLEEPY";

        case NautyState::SLEEPING:
            return "SLEEPING";

        case NautyState::CELEBRATING:
            return "CELEBRATING";

        default:
            return "UNKNOWN";
    }
}