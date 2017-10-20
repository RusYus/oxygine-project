#pragma once

#include "math/Rect.h"

#include "IDrawable.hpp"

DECLARE_SMART(Static, spStatic);
class Static : public oxygine::Actor, public IDrawable
{
public:
    Static(const oxygine::Rect& a_Rc)
    {
        Type = Service::ObjectType::StaticBody;
        m_Box = new Sprite;
        m_Box->setResAnim(res::ui.getResAnim("pen"));
        m_Box->setSize(Service::Convert(a_Rc.getSize()));
        m_Box->attachTo(m_View);

        m_View->setSize(m_Box->getSize());
        m_Position.set(a_Rc.getX(), a_Rc.getY());
        m_View->setPosition(Service::Convert(m_Position));

        addChild(m_View);
    }

    spActor GetView() const
    {
        return m_View;
    }
    void doRender(const oxygine::RenderState& rs) {}
};
