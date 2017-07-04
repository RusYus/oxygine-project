#pragma once

#include "BasisEventProxy.hpp"
#include "BasisEvents.hpp"
#include "Button.h"

#include <iostream>

DECLARE_SMART(MovementButton, spMoveButton);
class MovementButton : public oxygine::Button
{
public:
    MovementButton(bool aRight, spEventProxy aEventProxy)
        : mIsRight(aRight)
    {
        mEventProxy = aEventProxy;

        addEventListener(oxygine::TouchEvent::TOUCH_DOWN, CLOSURE(this, &MovementButton::Move));
        addEventListener(oxygine::TouchEvent::TOUCH_UP, CLOSURE(this, &MovementButton::Stop));
    }

private:
    void Move(Event*)
    {
        if (!mIsMoving)
        {
            mIsMoving = true;
        }

        SendEvent();
    }

    void Stop(Event*)
    {
        if (mIsMoving)
        {
            mIsMoving = false;
        }

        SendEvent();
    }

    void SendEvent()
    {
        std::cout << "Sending event..." << std::endl;
        PlayerMoveEvent event(mIsMoving, mIsRight);
        mEventProxy->dispatchEvent(&event);
    }

private:
    spEventProxy mEventProxy;
    bool mIsMoving;
    bool mIsRight;
};

DECLARE_SMART(JumpButton, spJumpButton);
class JumpButton : public oxygine::Button
{
public:
    JumpButton(spEventProxy aEventProxy)
    {
        mEventProxy = aEventProxy;

        addEventListener(oxygine::TouchEvent::TOUCH_DOWN, CLOSURE(this, &JumpButton::Jump));
    }

private:
    void Jump(Event*)
    {
        std::cout << "Sending event..." << std::endl;
        PlayerJumpEvent event;
        mEventProxy->dispatchEvent(&event);
    }

private:
    spEventProxy mEventProxy;
};
