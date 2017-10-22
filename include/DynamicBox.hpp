#pragma once

#include "ICarrier.hpp"
#include "IDebugDrawable.hpp"
#include "BasisEventProxy.hpp"

DECLARE_SMART(DynamicBox, spDynamicBox);
class DynamicBox: public oxygine::Actor, public virtual ICarrier, public virtual IDebugDrawable
{
public:
    DynamicBox(const std::shared_ptr<ICollisionManager>&);
    ~DynamicBox();
    void Update();
    void SetPosition() override;
    void CheckCollisions() override;
    void AddDirection(const Service::Vector2L&) override;
    void doRender(const oxygine::RenderState&);
};
