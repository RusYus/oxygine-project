#pragma once

#include <vector>

#include "Actor.h"
#include "RenderState.h"
#include "core/gl/ShaderProgramGL.h"
#include "core/VideoDriver.h"
#include "core/gl/VideoDriverGLES20.h"
#include "Material.h"
#include "STDMaterial.h"

#include "BasisObject.hpp"

class IDrawable : public Basis::BasisObject
{
public:
    IDrawable()
        : m_View(new oxygine::Actor)
        , m_Box(new oxygine::Sprite)
    {
        m_Box->attachTo(m_View);

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

    virtual ~IDrawable()
    {
        if (m_ShaderProgram)
        {
            delete m_ShaderProgram;
        }
    }

    inline float GetX() const override { return (m_View.get() ? m_View->getX() : .0f); }
    inline float GetY() const override { return (m_View.get() ? m_View->getY() : .0f); }
    inline float GetWidth() const override { return (m_View.get() ? m_View->getWidth() : .0f); }
    inline float GetHeight() const override { return (m_View.get() ? m_View->getHeight() : .0f); }
    inline oxygine::Vector2 GetPosition() const override { return m_View->getPosition(); }
    void SetDebugDraw(bool a_Value) { m_DebugDraw = a_Value; }
    bool GetDebugDraw() const { return m_DebugDraw; }

protected:
    oxygine::spActor m_View;
    oxygine::spSprite m_Box;

    static const int MAX_VERTICES = 64;
    static const int CIRCLE_SEGMENTS = 16;
    oxygine::Vector2 m_Vertices[MAX_VERTICES];
    bool m_DebugDraw = false;
    typename oxygine::ShaderProgramGL* m_ShaderProgram;
};
