#include "RenderWidget.h"
#include "Constants.h"

#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "shaders/quad.glsl.hpp"
#include "shaders/raytracer.glsl.hpp"


// REMOVEME
#include <iostream>

namespace 
{
    void CompileShaders(Program& quadProgram, Program& computeProgram)
    {
        quadProgram.link(
            VertexShader(qtwin::GLVersion, {shaders::quad}),
            FragmentShader(qtwin::GLVersion, {shaders::quad})
        );
        computeProgram.link(
            ComputeShader(
                qtwin::GLVersion, {shaders::raytracer})
        );
    }

    void CreateTexture(QSize size, GLuint& tex)
    {
        if (tex) { glDeleteTextures(1, &tex); };

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA32F,
            size.width(), size.height(),
            0,
            GL_RGBA,
            GL_FLOAT,
            nullptr
        );
    }
}

namespace qtwin
{
    RenderWidget::RenderWidget(
        const CameraIntrinsics& cameraIntrinsics,
        const QGLFormat& format
        )
    : QGLWidget(format, static_cast<QWidget*>(nullptr)),
      m_intrinsics(cameraIntrinsics),
      m_boardTransform(1.0f)
    {
        grabKeyboard();
    }

    void RenderWidget::SetChessboardRotation(float yaw, float pitch, float roll)
    {
        m_boardTransform =
            m_boardTransform * glm::eulerAngleXYZ(yaw, pitch, roll);
    }

    void RenderWidget::SetChessboardTranslation(float x, float y, float z)
    {
        m_boardTransform[0][3] += x;
        m_boardTransform[1][3] += y;
        m_boardTransform[2][3] += z;
    }

    RenderWidget::~RenderWidget() 
    {
        releaseKeyboard();
    }

    void RenderWidget::initializeGL()
    {
        GLint major;
        GLint minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        if (major * 100 + minor * 10 < GLVersion)
        {
            throw std::runtime_error(
                "OpenGL version " + std::to_string(GLVersion) +
                " is required.");
        }

        CompileShaders(m_progQuad, m_progCompute);

        glGenVertexArrays(1, &m_vao); 
    }

    void RenderWidget::resizeGL(int width, int height)
    {
        glViewport(0, 0, width, height);
        m_curSize = QSize(width, height);
    }

    void RenderWidget::paintGL()
    {
        if (m_curSize != m_texSize)
        {
            CreateTexture(m_curSize, m_tex);
            m_texSize = m_curSize;
        }

        m_progCompute.use();

        glUniform1f(m_progCompute.uniform("f"), m_intrinsics.GetFocalLength());
        glUniform2fv(
            m_progCompute.uniform("p"),
            1,
            glm::value_ptr(m_intrinsics.GetPrincipalPoint()));
        glUniform2fv(
            m_progCompute.uniform("s"),
            1,
            glm::value_ptr(m_intrinsics.GetSpatialResolution()));

        glUniform3fv(
            m_progCompute.uniform("k"),
            1,
            glm::value_ptr(m_intrinsics.GetRadialDistortion()));

        glUniform2fv(
            m_progCompute.uniform("t"),
            1,
            glm::value_ptr(m_intrinsics.GetTangentialDistortion()));

        glUniformMatrix4fv(
            m_progCompute.uniform("R"),
            1,
            false, // Transpose
            glm::value_ptr(m_boardTransform));

	    glBindImageTexture(0, m_tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	    int lsize[3];
	    m_progCompute.localSize(lsize);

	    int ngroups[3];
	    ngroups[0] = (m_texSize.width() + lsize[0]-1) / lsize[0];
	    ngroups[1] = (m_texSize.height() + lsize[1]-1) / lsize[1];
	    ngroups[2] = 1;

	    glDispatchCompute(ngroups[0], ngroups[1], ngroups[2]);

	    // Prevent sampling before all writes to texture are done
	    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        m_progQuad.use();

        glUniform1i(m_progQuad.uniform("sampler"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_tex);

        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    void RenderWidget::keyPressEvent(QKeyEvent* pEvent)
    {
        switch(pEvent->key())
        {
            case Qt::Key_Q:
                parentWidget()->close();
            default: break;
        }

        updateGL();
    }
}

