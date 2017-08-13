#pragma once

#include "math/Rect.h"

#include "BasisObject.hpp"

DECLARE_SMART(Static, spStatic);
class Static : public oxygine::Actor, public Basis::BasisObject
{
public:
    Static(const oxygine::RectF& a_Rc)
        : mBodyPair(Service::ObjectType::Ground, this)
    {
//        setResAnim(res::ui.getResAnim("pen"));
//        setPosition(a_Rc.getLeftTop());
        mBox = new Sprite;
        mBox->setResAnim(res::ui.getResAnim("pen"));
        mBox->setSize(a_Rc.getSize());
        mBox->attachTo(m_View);

        m_View->setSize(mBox->getSize());
        m_View->setPosition(a_Rc.getLeftTop());

        addChild(m_View);

    }

    spActor GetView() const
    {
        return m_View;
    }
    void doRender(const oxygine::RenderState& rs) {}

public:
    std::pair<Service::ObjectType, Static*> mBodyPair;
    oxygine::spSprite mBox;
};
