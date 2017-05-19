#include "BasisCamera.hpp"
#include "BasisEvents.hpp"

#include <map>

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

void Camera::onEvent(Event* aEvent)
{
    CameraMovementEvent* cameraEvent = safeCast<CameraMovementEvent*>(aEvent);
    const Vector2& pos = cameraEvent->_movement;
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
