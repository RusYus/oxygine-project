#pragma once

#include "Button.h"
#include "PlayerEventProxy.h"
#include "BasicEvents.h"

#include <iostream>

using namespace oxygine;

DECLARE_SMART(MovementButton, spMoveButton);
class MovementButton : public Button
{
public:
    MovementButton(bool aRight, spEventProxy aEventProxy)
        : _isRight(aRight)
    {
        _eventProxy = aEventProxy;

        addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &MovementButton::Move));
        addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &MovementButton::Stop));
    }

private:
    void Move(Event*)
    {
        if (!_isMoving)
        {
            _isMoving = true;
        }

        SendEvent();
    }

    void Stop(Event*)
    {
        if (_isMoving)
        {
            _isMoving = false;
        }

        SendEvent();
    }

    void SendEvent()
    {
        std::cout << "Sending event..." << std::endl;
        PlayerMoveEvent event(_isMoving, _isRight);
        _eventProxy->dispatchEvent(&event);
    }

private:
    spEventProxy _eventProxy;
    bool _isMoving;
    bool _isRight;
};

DECLARE_SMART(JumpButton, spJumpButton);
class JumpButton : public Button
{
public:
    JumpButton(spEventProxy aEventProxy)
    {
        _eventProxy = aEventProxy;

        addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &JumpButton::Jump));
    }

private:
    void Jump(Event*)
    {
        std::cout << "Sending event..." << std::endl;
        PlayerJumpEvent event;
        _eventProxy->dispatchEvent(&event);
    }

private:
    spEventProxy _eventProxy;
};
