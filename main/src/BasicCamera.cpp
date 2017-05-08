#include <map>

#include "BasicCamera.h"
#include "BasicEvents.h"

#include <iostream>

Camera::Camera(spEventProxy aEventProxy)
{
    aEventProxy->addEventListener(CameraMovementEvent::EVENT, CLOSURE(this, &Camera::onEvent));

    _transform.identity();
}

void Camera::setContent(spActor content)
{
    _content = content;
    addChild(content);
}

void Camera::onEvent(Event* ev)
{
    CameraMovementEvent* pe = safeCast<CameraMovementEvent*>(ev);
    const Vector2& pos = pe->_movement;
    _transform.translate(-Vector3(pos.x, pos.y, 0));
    update();
}

void Camera::doUpdate(const UpdateState& /*us*/)
{
}

void Camera::update()
{
    Transform tr(_transform);
    _content->setTransform(tr);
}
