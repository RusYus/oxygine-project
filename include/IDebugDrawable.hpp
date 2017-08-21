#pragma once

#include <vector>

#include "RenderState.h"
#include "core/gl/ShaderProgramGL.h"
#include "core/VideoDriver.h"
#include "core/gl/VideoDriverGLES20.h"
#include "Material.h"
#include "STDMaterial.h"
#include "math/ScalarMath.h"

#include "IDrawable.hpp"
#include "CollisionInfo.hpp"

class IDebugDrawable : public virtual IDrawable
{
public:
    IDebugDrawable()
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

        int vs = oxygine::ShaderProgramGL::createShader(GL_VERTEX_SHADER, vertexShaderData, 0, 0);
        int fs = oxygine::ShaderProgramGL::createShader(GL_FRAGMENT_SHADER, fragmentShaderData, 0, 0);

        int pr = oxygine::ShaderProgramGL::createProgram(
            vs,
            fs,
            (oxygine::VertexDeclarationGL*)oxygine::IVideoDriver::instance->getVertexDeclaration(oxygine::VERTEX_POSITION));
        m_ShaderProgram = new oxygine::ShaderProgramGL(pr);
    }

    virtual ~IDebugDrawable()
    {
        if (m_ShaderProgram)
        {
            delete m_ShaderProgram;
        }
    }

    void SetDebugDraw(bool a_Value) { m_DebugDraw = a_Value; }
    bool GetDebugDraw() const { return m_DebugDraw; }

protected:
    void CreateCircleVertices(const oxygine::Vector2& a_Center, int a_Radius)
    {
        const float k_increment = 2.0f * MATH_PI / CIRCLE_SEGMENTS;
        float theta = 0.0f;

        for (int i = 0; i < CIRCLE_SEGMENTS; ++i)
        {
            oxygine::Vector2 v  = oxygine::Vector2(oxygine::scalar::cos(theta), oxygine::scalar::sin(theta));
            v *=a_Radius;
            v += a_Center;
            m_Vertices[i] = v;
            theta += k_increment;
        }
    }

    void DrawPrimitives(int a_Count, const oxygine::Color& a_Color)
    {
        oxglEnableVertexAttribArray(0);
        oxglVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLfloat*)m_Vertices);

        oxygine::Vector4 c(a_Color.r, a_Color.g, a_Color.b, 1.0f);
        oxygine::IVideoDriver::instance->setUniform("color", &c, 1);
        glDrawArrays(GL_LINE_LOOP, 0, a_Count);

        oxglDisableVertexAttribArray(0);
    }

    void DrawCollisionRays(
        const std::shared_ptr<std::vector<Collision::Ray>>& a_Rays,
        const oxygine::Transform& a_Transform,
        oxygine::Color a_Color = oxygine::Color::Green)
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
        for (auto& ray : *a_Rays)
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
                DrawPrimitives(2, a_Color);
            }
            else
            {
                CreateCircleVertices(ray.Original, 3);
                DrawPrimitives(CIRCLE_SEGMENTS, oxygine::Color::Green);
            }
        }
    }

protected:
    static const int MAX_VERTICES = 64;
    static const int CIRCLE_SEGMENTS = 16;
    oxygine::Vector2 m_Vertices[MAX_VERTICES];
    bool m_DebugDraw = false;
    typename oxygine::ShaderProgramGL* m_ShaderProgram;
};
