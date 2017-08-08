#include "BasisCamera.hpp"
#include "BasisEvents.hpp"

#include <map>

Camera::Camera(spEventProxy aEventProxy)
{
    aEventProxy->addEventListener(CameraMovementEvent::EVENT, CLOSURE(this, &Camera::onEvent));

    mTransform.identity();
}

void Camera::setContent(oxygine::spActor content)
{
    mContent = content;
    addChild(content);
}

void Camera::onEvent(oxygine::Event* aEvent)
{
    CameraMovementEvent* cameraEvent = oxygine::safeCast<CameraMovementEvent*>(aEvent);
    const oxygine::Vector2& pos = cameraEvent->mMovement;
    mTransform.translate(-oxygine::Vector3(pos.x, pos.y, 0));
    update();
}

void Camera::doUpdate(const oxygine::UpdateState& /*us*/)
{
}

void Camera::update()
{
    oxygine::Transform tr(mTransform);
    mContent->setTransform(tr);
}
