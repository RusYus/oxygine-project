#pragma once

#include "BasisEvents.hpp"
#include "BasisEventProxy.hpp"
#include "oxygine-framework.h"

#include <map>
using namespace oxygine;

DECLARE_SMART(Camera, spCamera);
class Camera : public Actor
{

public:
    struct touch
    {
        Vector2 current;
        Vector2 previous;
    };

    Camera(spEventProxy);

    void setContent(spActor);
    void onEvent(Event*);
    void doUpdate(const UpdateState&);
    void update();

    std::map<int, touch> _touches;
    spActor _content;
    Matrix _transform;
};
