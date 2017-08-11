#include <iostream>

#include "core/VideoDriver.h"
#include "core/gl/VideoDriverGLES20.h"
#include "Material.h"
#include "STDMaterial.h"

#include "SDL_keyboard.h"
#include "Player.hpp"
#include "DemoLevel.hpp"
#include "res.hpp"
#include "BasisEvents.hpp"


Player::Player()
    : mBodyPair(Service::ObjectType::Player, this)
    , mCollisionNormal(0, 0)
    , mIsButtonMoving(false)
    , mIsJumping(false)
{
    const char* vertexShaderData = "\
        uniform mediump mat4 projection;\
        attribute vec2 a_position;\
        void main() {\
        vec4 position = vec4(a_position, 0.0, 1.0);\
        gl_Position = projection * position;\
        }\
        ";

    const char* fragmentShaderData = "\
        uniform mediump vec4 color;\
        void main() { \
        gl_FragColor = color; \
        } \
        ";

    int vs = ShaderProgramGL::createShader(GL_VERTEX_SHADER, vertexShaderData, 0, 0);
    int fs = ShaderProgramGL::createShader(GL_FRAGMENT_SHADER, fragmentShaderData, 0, 0);

    int pr = ShaderProgramGL::createProgram(vs, fs, (VertexDeclarationGL*)IVideoDriver::instance->getVertexDeclaration(VERTEX_POSITION));
    _program = new ShaderProgramGL(pr);
}

Player::~Player()
{
    delete _program;
    mRays.clear();
}

spActor Player::GetView() const
{
    return mView;
}

void Player::Init(spEventProxy aEventProxy)
{
    mView = new Actor;
    oxygine::Vector2 pos = getStage()->getSize() / 2;



    mBox = new Sprite;
    mBox->setResAnim(res::ui.getResAnim("player"));
    mBox->attachTo(mView);
//    mBox->setAnchor(Vector2(0.5f, 0.5f));
    mView->setPosition(100, 100);
    mView->setSize(mBox->getSize());

    addChild(mView);

    mEventProxy = aEventProxy;

    mEventProxy->addEventListener(PlayerMoveEvent::EVENT, CLOSURE(this, &Player::ProcessMoveEvent));

    mEventProxy->addEventListener(PlayerJumpEvent::EVENT, CLOSURE(this, &Player::Jump));

    mDirection = oxygine::Vector2();

    int actualIntervalsNumber = static_cast<int>(std::ceil(GetWidth() / Service::Constants::RAYCAST_INTERVAL));
    float actualIntervalLength = GetWidth() / actualIntervalsNumber;
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Bottom
        mRays.emplace_back(Collision::Ray(oxygine::Vector2(GetX() + i * actualIntervalLength, GetY() + GetHeight()),
                                          oxygine::Vector2(GetX() + i * actualIntervalLength, GetY() + GetHeight()),
                                          Collision::RayDirection::Down));
        // Top
        mRays.emplace_back(Collision::Ray(oxygine::Vector2(GetX() + i * actualIntervalLength, GetY()),
                                          oxygine::Vector2(GetX() + i * actualIntervalLength, GetY()),
                                          Collision::RayDirection::Up));
    }

    actualIntervalsNumber = static_cast<int>(std::ceil(GetHeight() / Service::Constants::RAYCAST_INTERVAL));
    actualIntervalLength = GetHeight() / actualIntervalsNumber;
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Right
        mRays.emplace_back(Collision::Ray(oxygine::Vector2(GetX() + GetWidth(), GetY() + i * actualIntervalLength),
                                          oxygine::Vector2(GetX() + GetWidth(), GetY() + i * actualIntervalLength),
                                          Collision::RayDirection::Right));
        // Left
        mRays.emplace_back(Collision::Ray(oxygine::Vector2(GetX(), GetY() + i * actualIntervalLength),
                                          oxygine::Vector2(GetX(), GetY() + i * actualIntervalLength),
                                          Collision::RayDirection::Left));
    }
}

void Player::Jump(Event* /*aEvent*/)
{
    std::cout << mCollisionNormal.y << std::endl;
    if (!mIsJumping)
    {
        std::cout << "Jumping!------------------------------------------------------------" << std::endl;
        mIsJumping = true;
        mDirection.y -= mJumpSpeed;
//        _body->SetLinearVelocity(b2Vec2(mDirection.x, -mJumpSpeed / Service::Constants::SCALE));
    }
}

void Player::ProcessMoveEvent(Event* aEvent)
{
    PlayerMoveEvent* playerEvent = safeCast<PlayerMoveEvent*>(aEvent);
    if (!playerEvent->mIsMoving)
    {
        mIsButtonMoving = false;
        Stop();
    }
    else
    {
        mIsButtonMoving = true;
        Move(playerEvent->mIsMovingRight);
    }
}

void Player::Move(bool aIsMovingRight)
{
//    if (_body != nullptr)
//    {
//        mDirection = _body->GetLinearVelocity();

////        std::cout << "Moving!" << "NORMALS:" << mNormal.x << ";" << mGroundNormal.x << std::endl;
        mDirection.x = aIsMovingRight ? mMaxSpeed : -mMaxSpeed;
//        mDirection.x /= Service::Constants::SCALE;

//        // Collision took place
//        if ((aIsMovingRight && mCollisionNormal.x > 0) || (!aIsMovingRight && mCollisionNormal.x < 0))
//        {
//            std::cout << "Collision took place!" << std::endl;
//            Stop();
//        }

//        _body->SetLinearVelocity(mDirection);
//    }
}

inline void Player::Stop()
{
    mDirection.x = 0;
}

void Player::SetY(float newPosY)
{
    mDirection.y = newPosY;
}

void Player::SetX(float newPosX)
{
    mDirection.x = newPosX;
}

void Player::SetDirection(const oxygine::Vector2& aNewDirection)
{
    mDirection = aNewDirection;
}

float Player::GetX() const
{
    return (mView.get() ? mView->getX() : .0f);
}

float Player::GetY() const
{
    return (mView.get() ? mView->getY() : .0f);
}

float Player::GetWidth() const
{
    return (mView.get() ? mView->getWidth() : .0f);
}

float Player::GetHeight() const
{
    return (mView.get() ? mView->getHeight() : .0f);
}

oxygine::Vector2 Player::GetRayOriginal() const
{
    return mRays.back().Original;
}

oxygine::Vector2 Player::GetRayDestination() const
{
    return mRays.back().Destination;
}
std::vector<Collision::Ray>& Player::GetRays()
{
    return mRays;
}

void Player::SetCollisionNormal(const oxygine::Vector2 aNormal)
{
//    std::cout << "Setting ground normal; old=" << mCollisionNormal.x << ":" << mCollisionNormal.y << ";new=" << aNormal.x << ":" << aNormal.y << std::endl;
//    mCollisionNormal += aNormal;
//    std::cout << "Ground:" << mCollisionNormal.x << ":" << mCollisionNormal.y << std::endl;

//    std::cout << "C:" << mCollisionNormal.y;
    mCollisionNormal += aNormal;
//    std::cout << " : " << mCollisionNormal.y << std::endl;

}

void Player::ResetCollisionNormal(const Collision::CollisionInfo& a_Sides)
{
    mCollisionNormal.SetZero();

    if (a_Sides.Down)
    {
        mCollisionNormal.y = -1;
    }

    if (a_Sides.Up)
    {
        mCollisionNormal.y = 1;
    }

    if (a_Sides.Right)
    {
        mCollisionNormal.x = 1;
    }

    if (a_Sides.Left)
    {
        mCollisionNormal.x = -1;
    }
}

void Player::ProcessKeyboard()
{
    const Uint8* states = SDL_GetKeyboardState(nullptr);

    if (states[SDL_SCANCODE_LEFT])
    {
        Move(false);
    }
    else if (states[SDL_SCANCODE_RIGHT])
    {
        Move(true);
    }
    // Might move by onscreen buttons.
    else if (!mIsButtonMoving)
    {
        Stop();
    }

    if (states[SDL_SCANCODE_SPACE])
    {
        Jump(nullptr);
    }
}

void Player::SetPosition()
{
    Vector2 newPos = mView->getPosition() + mDirection;

//    std::cout << "Pos: " << newPos.x << " : " << newPos.y << std::endl;

    for(auto& ray : mRays)
    {
        ray.Original += mDirection;

        switch (ray.Direction)
        {
            case Collision::RayDirection::Up:
                if (mDirection.y < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y + mDirection.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;
            case Collision::RayDirection::Down:
                if (mDirection.y > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y + mDirection.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;
            case Collision::RayDirection::Right:
                if (mDirection.x > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x + mDirection.x, ray.Original.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;

            case Collision::RayDirection::Left:
                if (mDirection.x < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x + mDirection.x, ray.Original.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;
        }
    }

//    std::cout << mRays.at(0).Original.x << ":" << mRays.at(0).Original.y
//              << "  |  " << mRays.at(0).Destination.x << ":" << mRays.at(0).Destination.y << std::endl;

    mView->setPosition(newPos);

    // If player doesn't stand on something, he can't jump.
    if (mCollisionNormal.y == -1)
    {
        mIsJumping = false;
    }
    else
    {
        mIsJumping = true;
    }

//        std::cout << "O: " << mRays.at(0).Destination.x - mRays.at(0).Original.x << ":" << mRays.at(0).Destination.y - mRays.at(0).Original.y << std::endl;

//    std::cout << "Hit: " << mRays.at(0).IsHitInLastStep << ":" << mRays.at(0).IsHitInCurrentStep << std::endl;


//        std::cout << "Player:"
//                  << mDirection.x << ":" << mDirection.y << "  |  "
//                  << mCollisionNormal.x << ":" << mCollisionNormal.y
//                  << std::endl;

}

oxygine::Vector2 Player::GetPosition() const
{
    return mView->getPosition();
}

oxygine::Vector2 Player::GetDirection() const
{
    return mDirection;
}

Service::Normal2 Player::GetCollisionNormal() const
{
    return mCollisionNormal;
}

void Player::Update(const UpdateState& us)
{
    ProcessKeyboard();

//    mDirection.y = _body->GetLinearVelocity().y;

    // Reseting direction, if collision in place.
    if ((mDirection.x < 0 && mCollisionNormal.x < 0) || (mDirection.x > 0  && mCollisionNormal.x > 0))
    {
//        std::cout << "In Update: dir.x = 0" << std::endl;
        mDirection.x = 0;
    }

//    _body->SetLinearVelocity(mDirection);

    mDirection.y += us.dt / Service::Constants::GRAVITY;

    for (auto& ray : mRays)
    {
        // Down  : y
        // Up    : y
        // Right : x
        // Left  : x

        switch (ray.Direction)
        {
            case Collision::RayDirection::Up:
                if (mDirection.y < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y + mDirection.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;
            case Collision::RayDirection::Down:
                if (mDirection.y > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y + mDirection.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;
            case Collision::RayDirection::Right:
                if (mDirection.x > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x + mDirection.x, ray.Original.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;

            case Collision::RayDirection::Left:
                if (mDirection.x < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x + mDirection.x, ray.Original.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;
        }
    }

//    std::cout << "O: " << mRays.at(0).Original.x << ":" << mRays.at(0).Original.y << std::endl;
//    std::cout << "D: " << mRays.at(0).Destination.x << ":" << mRays.at(0).Destination.y << std::endl;



//    std::cout << "Dt:" << us.dt << std::endl;

//    std::cout << "Player:"
//              << mCollisionNormal.x << ":" << mCollisionNormal.y << std::endl;
//              << mView->getX() << ":" << mView->getY() << std::endl;

//    b2Vec2 b2pos = _body->GetPosition();
//    Vector2 pos = Service::Utils::Convert(b2pos);
//    CameraMovementEvent event(pos - mView->getPosition());
//    mView->setPosition(pos);
//    mEventProxy->dispatchEvent(&event);

//    std::cout << "Player: " << mGroundNormal.x << ":" << mGroundNormal.y << std::endl;
}

void Player::SetDebugDraw(bool a_Value)
{
    m_DebugDraw = a_Value;
}

bool Player::GetDebugDraw() const
{
    return m_DebugDraw;
}

void Player::doRender(const oxygine::RenderState& rs)
{
    if (!m_DebugDraw)
    {
        return;
    }

    Material::setCurrent(0);

    IVideoDriver* driver = IVideoDriver::instance;

    driver->setShaderProgram(_program);

    Matrix m = Matrix(rs.transform) * STDMaterial::instance->getRenderer()->getViewProjection();
    driver->setUniform("projection", &m);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto& ray : mRays)
    {
        if (ray.Original != ray.Destination)
        {
            mVertices[0] = ray.Original;
            // actual difference is to small, so I need to increase it visually.
            oxygine::Vector2 diff = ray.Destination - ray.Original;
            if (diff.x > 0)
                diff.x += 20;
            if (diff.x < 0)
                diff.x -= 20;
            if (diff.y > 0)
                diff.y += 20;
            if (diff.y < 0)
                diff.y -= 20;
    //        mVertices[1] = ray.Destination;
            mVertices[1] = ray.Original + diff;
            drawPrimitives(2, oxygine::Color::Green);
        }
        else
        {
            createCircleVertices(ray.Original, 3);
            drawPrimitives(CIRCLE_SEGMENTS, oxygine::Color::Green);
        }
    }
}

void Player::createCircleVertices(const oxygine::Vector2& center, int aRadius)
{
    int vertexCount = 16;
    const float k_increment = 2.0f * b2_pi / CIRCLE_SEGMENTS;
    float theta = 0.0f;

    for (int32 i = 0; i < CIRCLE_SEGMENTS; ++i)
    {
        oxygine::Vector2 v  = oxygine::Vector2(scalar::cos(theta), scalar::sin(theta));
        v *=aRadius;
        v += center;
        mVertices[i] = v;
        theta += k_increment;
    }
}

void Player::drawPrimitives(int count, const oxygine::Color& color)
{
    oxglEnableVertexAttribArray(0);
    oxglVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLfloat*)mVertices);

    Vector4 c(color.r, color.g, color.b, 1.0f);
    IVideoDriver::instance->setUniform("color", &c, 1);
    glDrawArrays(GL_LINE_LOOP, 0, count);

    oxglDisableVertexAttribArray(0);
}
