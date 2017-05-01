#pragma once

#include "Event.h"

using namespace oxygine;

class PlayerMovementEvent : public Event
{
public:
    PlayerMovementEvent(Vector2 aMovement)
        :Event(EVENT)
        , movement(aMovement)
    {
    }

public:
    enum { EVENT = eventID('M','o','v','e') };
    Vector2 movement;
};
