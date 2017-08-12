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
    m_Rays.clear();
}

spActor Player::GetView() const
{
    return m_View;
}

void Player::Init(spEventProxy aEventProxy)
{
//    m_View = new Actor;
    oxygine::Vector2 pos = getStage()->getSize() / 2;



    mBox = new Sprite;
    mBox->setResAnim(res::ui.getResAnim("player"));
    mBox->attachTo(m_View);
//    mBox->setAnchor(Vector2(0.5f, 0.5f));
    m_View->setPosition(100, 100);
    m_View->setSize(mBox->getSize());

    addChild(m_View);

    mEventProxy = aEventProxy;

    mEventProxy->addEventListener(PlayerMoveEvent::EVENT, CLOSURE(this, &Player::ProcessMoveEvent));

    mEventProxy->addEventListener(PlayerJumpEvent::EVENT, CLOSURE(this, &Player::Jump));

    m_Direction = oxygine::Vector2();

    SetRays();
}

void Player::Jump(Event* /*aEvent*/)
{
    std::cout << m_CollisionNormal.y << std::endl;
    if (!mIsJumping)
    {
        std::cout << "Jumping!------------------------------------------------------------" << std::endl;
        mIsJumping = true;
        m_Direction.y -= mJumpSpeed;
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
        m_Direction.x = aIsMovingRight ? m_MaxSpeed : -m_MaxSpeed;
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
    m_Direction.x = 0;
}

//void Player::SetDirection(const oxygine::Vector2& aNewDirection)
//{
//    m_Direction = aNewDirection;
//}

//oxygine::Vector2 Player::GetRayOriginal() const
//{
//    return m_Rays.back().Original;
//}

//oxygine::Vector2 Player::GetRayDestination() const
//{
//    return m_Rays.back().Destination;
//}
//std::vector<Collision::Ray>& Player::GetRays()
//{
//    return m_Rays;
//}

//void Player::SetCollisionNormal(const oxygine::Vector2 aNormal)
//{
////    std::cout << "Setting ground normal; old=" << mCollisionNormal.x << ":" << mCollisionNormal.y << ";new=" << aNormal.x << ":" << aNormal.y << std::endl;
////    mCollisionNormal += aNormal;
////    std::cout << "Ground:" << mCollisionNormal.x << ":" << mCollisionNormal.y << std::endl;

////    std::cout << "C:" << mCollisionNormal.y;
//    m_CollisionNormal += aNormal;
////    std::cout << " : " << mCollisionNormal.y << std::endl;

//}

//void Player::ResetCollisionNormal(const Collision::CollisionInfo& a_Sides)
//{
//    m_CollisionNormal.SetZero();

//    if (a_Sides.Down)
//    {
//        m_CollisionNormal.y = -1;
//    }

//    if (a_Sides.Up)
//    {
//        m_CollisionNormal.y = 1;
//    }

//    if (a_Sides.Right)
//    {
//        m_CollisionNormal.x = 1;
//    }

//    if (a_Sides.Left)
//    {
//        m_CollisionNormal.x = -1;
//    }
//}

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
    IMovable::SetPosition();
    // If player doesn't stand on something, he can't jump.
    if (m_CollisionNormal.y == -1)
    {
        mIsJumping = false;
    }
    else
    {
        mIsJumping = true;
    }

//        std::cout << "O: " << mRays.at(0).Destination.x - mRays.at(0).Original.x << ":" << mRays.at(0).Destination.y - mRays.at(0).Original.y << std::endl;

//    std::cout << "Hit: " << mRays.at(0).IsHitInLastStep << ":" << mRays.at(0).IsHitInCurrentStep << std::endl;


        std::cout << "Player:"
                  << m_Direction.x << ":" << m_Direction.y << "  |  "
                  << m_CollisionNormal.x << ":" << m_CollisionNormal.y
                  << std::endl;

}

void Player::Update(const UpdateState& us)
{
    ProcessKeyboard();

//    mDirection.y = _body->GetLinearVelocity().y;

    // Reseting direction, if collision in place.
    if ((m_Direction.x < 0 && m_CollisionNormal.x < 0) || (m_Direction.x > 0  && m_CollisionNormal.x > 0))
    {
//        std::cout << "In Update: dir.x = 0" << std::endl;
        m_Direction.x = 0;
    }

//    _body->SetLinearVelocity(mDirection);

    m_Direction.y += us.dt / static_cast<float>(Service::Constants::GRAVITY);

    std::cout << "Update:" << m_Direction.x << ":" << m_Direction.y << std::endl;

    UpdateRays(false);

//    std::cout << "O: " << mRays.at(0).Original.x << ":" << mRays.at(0).Original.y << std::endl;
//    std::cout << "D: " << mRays.at(0).Destination.x << ":" << mRays.at(0).Destination.y << std::endl;



//    std::cout << "Dt:" << us.dt << std::endl;

//    std::cout << "Player:"
//              << mCollisionNormal.x << ":" << mCollisionNormal.y << std::endl;
//              << m_View->getX() << ":" << m_View->getY() << std::endl;

//    b2Vec2 b2pos = _body->GetPosition();
//    Vector2 pos = Service::Utils::Convert(b2pos);
//    CameraMovementEvent event(pos - m_View->getPosition());
//    m_View->setPosition(pos);
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
    for (auto& ray : m_Rays)
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
