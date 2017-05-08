#pragma once

#include "Event.h"

using namespace oxygine;

class CameraMovementEvent : public Event
{
public:
    CameraMovementEvent(Vector2 aMovement)
        : Event(EVENT)
        , _movement(aMovement)
    {
    }

public:
    enum { EVENT = eventID('C','a','m','M') };
    Vector2 _movement;
};

class PlayerMovementEvent : public Event
{
public:
    PlayerMovementEvent(bool aMoving = false, bool aMovingRight = true)
        : Event(EVENT)
        , _isMoving(aMoving)
        , _isMovingRight(aMovingRight)
    {
    }

public:
    enum { EVENT = eventID('P','l','a','M') };
    bool _isMoving;
    bool _isMovingRight;
};
