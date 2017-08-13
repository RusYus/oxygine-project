#pragma once

#include "math/Rect.h"

#include "IDrawable.hpp"

DECLARE_SMART(Static, spStatic);
class Static : public oxygine::Actor, public IDrawable
{
public:
    Static(const oxygine::RectF& a_Rc)
        : mBodyPair(Service::ObjectType::Ground, this)
    {
//        setResAnim(res::ui.getResAnim("pen"));
//        setPosition(a_Rc.getLeftTop());
        m_Box = new Sprite;
        m_Box->setResAnim(res::ui.getResAnim("pen"));
        m_Box->setSize(a_Rc.getSize());
        m_Box->attachTo(m_View);

        m_View->setSize(m_Box->getSize());
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
};
