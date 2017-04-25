#pragma once

#include <map>

#include "oxygine-framework.h"

using namespace oxygine;

class Camera : public Actor
{

public:
    struct touch
    {
        Vector2 current;
        Vector2 previous;
    };

    Camera();

    void setContent(spActor);
    void onEvent(Event*);
    void doUpdate(const UpdateState&);
    void update();

    std::map<int, touch> _touches;
    spActor _content;
    Matrix _transform;
};
