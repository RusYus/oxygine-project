#pragma once

#include <vector>

#include "Actor.h"

#include "BasisObject.hpp"

class IDrawable : public Basis::BasisObject
{
public:
    IDrawable()
        : m_View(new oxygine::Actor)
        , m_Box(new oxygine::Sprite)
    {
        m_Box->attachTo(m_View);
    }

    virtual ~IDrawable() = default;

    inline float GetWidth() const override { return (m_View.get() ? m_View->getWidth() : .0f); }
    inline float GetHeight() const override { return (m_View.get() ? m_View->getHeight() : .0f); }

protected:
    oxygine::spActor m_View;
    oxygine::spSprite m_Box;
};
