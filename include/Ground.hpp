#pragma once

#include "Actor.h"
#include "math/Rect.h"

#include "BasisObject.hpp"

class Static : public oxygine::Actor, public Basis::BasisObject
{
public:
    Static(const oxygine::RectF& a_Rc)
        : mBodyPair(Service::ObjectType::Ground, this)
    {
//        setResAnim(res::ui.getResAnim("pen"));
        m_View->setSize(a_Rc.getSize());
        //    setPosition(rc.getLeftTop());
        m_View->setPosition(a_Rc.getLeftTop());
    }

public:
    std::pair<Service::ObjectType, Static*> mBodyPair;
};
