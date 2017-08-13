#pragma once

#include <vector>

#include "Actor.h"

#include "BasisObject.hpp"

class IDrawable : public Basis::BasisObject
{
public:
    IDrawable()
        : m_View(new oxygine::Actor)
    {}

    virtual ~IDrawable() = default;

    inline float GetX() const override { return (m_View.get() ? m_View->getX() : .0f); }
    inline float GetY() const override { return (m_View.get() ? m_View->getY() : .0f); }
    inline float GetWidth() const override { return (m_View.get() ? m_View->getWidth() : .0f); }
    inline float GetHeight() const override { return (m_View.get() ? m_View->getHeight() : .0f); }
    inline oxygine::Vector2 GetPosition() const override { return m_View->getPosition(); }

protected:
    oxygine::spActor m_View;
};
