#pragma once

#include "ICarrier.hpp"
#include "IDebugDrawable.hpp"
#include "BasisEventProxy.hpp"
#include "Utils.hpp"

DECLARE_SMART(DynamicBox, spDynamicBox);
class DynamicBox: public oxygine::Actor, public virtual ICarrier, public virtual IDebugDrawable
{
public:
    DynamicBox();
    ~DynamicBox();
    void Update();
    void SetPosition() override;
    void CheckCollisions() override;
    void doRender(const oxygine::RenderState&);
};
