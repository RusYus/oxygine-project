#include "IMovable.hpp"

void IMovable::SetDirection(const oxygine::Vector2& aNewDirection)
{
    m_Direction = aNewDirection;
}

oxygine::Vector2 IMovable::GetRayOriginal() const
{
    return m_Rays.back().Original;
}

oxygine::Vector2 IMovable::GetRayDestination() const
{
    return m_Rays.back().Destination;
}
std::vector<Collision::Ray>& IMovable::GetRays()
{
    return m_Rays;
}

oxygine::Vector2 IMovable::GetDirection() const
{
    return m_Direction;
}

void IMovable::SetCollisionNormal(const oxygine::Vector2 aNormal)
{
    m_CollisionNormal += aNormal;
}

void IMovable::ResetCollisionNormal(const Collision::CollisionInfo& a_Sides)
{
    m_CollisionNormal.SetZero();

    if (a_Sides.Down)
    {
        m_CollisionNormal.y = -1;
    }

    if (a_Sides.Up)
    {
        m_CollisionNormal.y = 1;
    }

    if (a_Sides.Right)
    {
        m_CollisionNormal.x = 1;
    }

    if (a_Sides.Left)
    {
        m_CollisionNormal.x = -1;
    }
}

void IMovable::SetPosition()
{
    oxygine::Vector2 newPos = GetPosition() + m_Direction;

    // TODO : Refactor!
    if (m_DebugDraw)
    {
        UpdateRays(true);
    }
    else
    {
        for(auto& ray : m_Rays)
        {
            ray.Original += m_Direction;
        }
    }

    m_View->setPosition(newPos);
}

void IMovable::UpdateRays(bool aOriginal)
{
    for(auto& ray : m_Rays)
    {
        if (aOriginal)
        {
            ray.Original += m_Direction;
        }

        ray.Destination = ray.Original;

        // TODO : Consider refactoring.
        switch (ray.Direction)
        {
            case Collision::RayDirection::Up:
                if (m_Direction.y < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y + m_Direction.y);
                }
                break;
            case Collision::RayDirection::Down:
                if (m_Direction.y > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y + m_Direction.y);
                }
                break;
            case Collision::RayDirection::Right:
                if (m_Direction.x > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x + m_Direction.x, ray.Original.y);
                }
                break;

            case Collision::RayDirection::Left:
                if (m_Direction.x < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x + m_Direction.x, ray.Original.y);
                }
                break;
            case Collision::RayDirection::UpLeft:
                if (m_Direction.x < 0 && m_Direction.y < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original + m_Direction);
                }
                break;
            case Collision::RayDirection::UpRight:
                if (m_Direction.x > 0 && m_Direction.y < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original + m_Direction);
                }
                break;
            case Collision::RayDirection::DownLeft:
                if (m_Direction.x < 0 && m_Direction.y > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original + m_Direction);
                }
                break;
            case Collision::RayDirection::DownRight:
                if (m_Direction.x > 0 && m_Direction.y > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original + m_Direction);
                }
                break;
        }
    }
}

void IMovable::SetRays()
{
    m_Rays.clear();

    int actualIntervalsNumber = static_cast<int>(std::ceil(GetWidth() / Service::Constants::RAYCAST_INTERVAL));
    float actualIntervalLength = GetWidth() / actualIntervalsNumber;
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Bottom
        m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX() + i * actualIntervalLength, GetY() + GetHeight()),
                                          oxygine::Vector2(GetX() + i * actualIntervalLength, GetY() + GetHeight()),
                                          Collision::RayDirection::Down));
        // Top
        m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX() + i * actualIntervalLength, GetY()),
                                          oxygine::Vector2(GetX() + i * actualIntervalLength, GetY()),
                                          Collision::RayDirection::Up));
    }

    actualIntervalsNumber = static_cast<int>(std::ceil(GetHeight() / Service::Constants::RAYCAST_INTERVAL));
    actualIntervalLength = GetHeight() / actualIntervalsNumber;
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Right
        m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX() + GetWidth(), GetY() + i * actualIntervalLength),
                                          oxygine::Vector2(GetX() + GetWidth(), GetY() + i * actualIntervalLength),
                                          Collision::RayDirection::Right));
        // Left
        m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX(), GetY() + i * actualIntervalLength),
                                          oxygine::Vector2(GetX(), GetY() + i * actualIntervalLength),
                                          Collision::RayDirection::Left));
    }

    // UpLeft diagonal.
    m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX(), GetY()),
                                       oxygine::Vector2(GetX(), GetY()),
                                       Collision::RayDirection::UpLeft));
    // UpRight diagonal.
    m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX() + GetWidth(), GetY()),
                                       oxygine::Vector2(GetX() + GetWidth(), GetY()),
                                       Collision::RayDirection::UpRight));
    // DownLeft diagonal.
    m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX(), GetY() + GetHeight()),
                                       oxygine::Vector2(GetX(), GetY() + GetHeight()),
                                       Collision::RayDirection::DownLeft));
    // DownRight diagonal.
    m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX() + GetWidth(), GetY() + GetHeight()),
                                       oxygine::Vector2(GetX() + GetWidth(), GetY() + GetHeight()),
                                       Collision::RayDirection::DownRight));
}

void IMovable::CreateCircleVertices(const oxygine::Vector2& a_Center, int a_Radius)
{
    const float k_increment = 2.0f * b2_pi / CIRCLE_SEGMENTS;
    float theta = 0.0f;

    for (int32 i = 0; i < CIRCLE_SEGMENTS; ++i)
    {
        oxygine::Vector2 v  = oxygine::Vector2(oxygine::scalar::cos(theta), oxygine::scalar::sin(theta));
        v *=a_Radius;
        v += a_Center;
        m_Vertices[i] = v;
        theta += k_increment;
    }
}

void IMovable::DrawPrimitives(int a_Count, const oxygine::Color& a_Color)
{
    oxglEnableVertexAttribArray(0);
    oxglVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLfloat*)m_Vertices);

    oxygine::Vector4 c(a_Color.r, a_Color.g, a_Color.b, 1.0f);
    oxygine::IVideoDriver::instance->setUniform("color", &c, 1);
    glDrawArrays(GL_LINE_LOOP, 0, a_Count);

    oxglDisableVertexAttribArray(0);
}

void IMovable::DrawDebugRays(const oxygine::Transform& a_Transform)
{
    if (!m_DebugDraw || !m_ShaderProgram)
    {
        return;
    }

    oxygine::Material::setCurrent(0);

    oxygine::IVideoDriver* driver = oxygine::IVideoDriver::instance;

    driver->setShaderProgram(m_ShaderProgram);

    oxygine::Matrix m = oxygine::Matrix(a_Transform) * oxygine::STDMaterial::instance->getRenderer()->getViewProjection();
    driver->setUniform("projection", &m);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto& ray : m_Rays)
    {
        if (ray.Original != ray.Destination)
        {
            m_Vertices[0] = ray.Original;
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
            m_Vertices[1] = ray.Original + diff;
            DrawPrimitives(2, oxygine::Color::Green);
        }
        else
        {
            CreateCircleVertices(ray.Original, 3);
            DrawPrimitives(CIRCLE_SEGMENTS, oxygine::Color::Green);
        }
    }
}
