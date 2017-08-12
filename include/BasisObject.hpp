#pragma once

#include "Actor.h"

#include "Utils.hpp"

namespace Basis
{

class BasisObject
{
public:
    BasisObject()
    {
        m_Id = Service::IdGenerator::GetNextId();
    }

    virtual ~BasisObject() = default;

    inline float GetX() const { return (m_View.get() ? m_View->getX() : .0f); }
    inline float GetY() const { return (m_View.get() ? m_View->getY() : .0f); }
    inline float GetWidth() const { return (m_View.get() ? m_View->getWidth() : .0f); }
    inline float GetHeight() const { return (m_View.get() ? m_View->getHeight() : .0f); }
    // TODO : what to do.
    //    void SetPosition();
    inline oxygine::Vector2 GetPosition() const { return m_View->getPosition(); }

    inline int GetId() const
    {
        return m_Id;
    }

protected:
    oxygine::spActor m_View;
    int m_Id;
};

}
