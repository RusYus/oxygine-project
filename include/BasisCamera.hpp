#pragma once

#include "BasisEvents.hpp"
#include "BasisEventProxy.hpp"
#include "oxygine-framework.h"

#include <map>

DECLARE_SMART(Camera, spCamera);
class Camera : public oxygine::Actor
{

public:
    struct touch
    {
        oxygine::Vector2 current;
        oxygine::Vector2 previous;
    };

    Camera(spEventProxy);

    void setContent(oxygine::spActor);
    void onEvent(oxygine::Event*);
    void doUpdate(const oxygine::UpdateState&);
    void update();

    std::map<int, touch> mTouches;
    oxygine::spActor mContent;
    oxygine::Matrix mTransform;
};
