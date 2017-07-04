#pragma once

#include "Event.h"

class CameraMovementEvent : public oxygine::Event
{
public:
    CameraMovementEvent(oxygine::Vector2 aMovement)
        : Event(EVENT)
        , mMovement(aMovement)
    {
    }

public:
    enum { EVENT = eventID('C','a','m','M') };
    oxygine::Vector2 mMovement;
};

class PlayerMoveEvent : public oxygine::Event
{
public:
    PlayerMoveEvent(bool aMoving = false, bool aMovingRight = true)
        : Event(EVENT)
        , mIsMoving(aMoving)
        , mIsMovingRight(aMovingRight)
    {
    }

public:
    enum { EVENT = eventID('P','l','a','M') };
    bool mIsMoving;
    bool mIsMovingRight;
};

class PlayerJumpEvent : public oxygine::Event
{
public:
    PlayerJumpEvent()
        : oxygine::Event(EVENT)
    {
    }

public:
    enum { EVENT = eventID('P','l','a','J') };
};
