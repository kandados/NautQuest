#pragma once

#include <Arduino.h>

/**
 * Estados visuales y de comportamiento de Nauty.
 *
 * Los estados IDLE, BLINK, LOOK_LEFT y LOOK_RIGHT forman parte
 * de su comportamiento cotidiano.
 *
 * Los demás estados se activarán debido a eventos concretos:
 * batería baja, misión completada, error, inactividad, etc.
 */
enum class NautyState : uint8_t
{
    IDLE,

    // Microcomportamientos
    BLINK,
    LOOK_LEFT,
    LOOK_RIGHT,

    // Estados expresivos
    HAPPY,
    SAD,
    ANGRY,
    CURIOUS,
    UNCERTAIN,
    TIRED,
    SLEEPY,
    SLEEPING,
    CELEBRATING
};

/**
 * Prioridad de una expresión.
 *
 * Una expresión de prioridad baja no puede interrumpir otra
 * expresión más importante.
 */
enum class NautyPriority : uint8_t
{
    BACKGROUND = 0,
    NORMAL = 1,
    IMPORTANT = 2,
    CRITICAL = 3
};

class NautyBehavior
{
public:
    void begin();
    void update();

    /**
     * Solicita un nuevo estado.
     *
     * @param state Estado que debe adoptar Nauty.
     * @param durationMs Duración del estado en milisegundos.
     * @param priority Prioridad frente al estado actual.
     * @return true si el estado fue aceptado.
     */
    bool requestState(
        NautyState state,
        uint32_t durationMs,
        NautyPriority priority = NautyPriority::NORMAL
    );

    /**
     * Fuerza el estado de reposo.
     */
    void returnToIdle();

    NautyState getState() const;
    NautyState getPreviousState() const;
    NautyPriority getPriority() const;

    uint32_t getStateElapsedTime() const;
    uint32_t getStateRemainingTime() const;

    bool isIdle() const;
    bool hasStateChanged();

    const char* getStateName() const;

private:
    NautyState currentState = NautyState::IDLE;
    NautyState previousState = NautyState::IDLE;

    NautyPriority currentPriority = NautyPriority::BACKGROUND;

    uint32_t stateStartedAt = 0;
    uint32_t stateDuration = 0;

    uint32_t nextIdleActionAt = 0;

    bool stateChanged = false;

    void changeState(
        NautyState newState,
        uint32_t durationMs,
        NautyPriority priority
    );

    void updateCurrentState(uint32_t now);
    void updateIdleBehavior(uint32_t now);
    void scheduleNextIdleAction(uint32_t now);

    static const char* stateToString(NautyState state);
};