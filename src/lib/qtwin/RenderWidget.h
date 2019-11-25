//
// QT render widget class.
//

#pragma once

#include <GL/gl.h>
#include <GL/glu.h>

#include <QGLFormat>
#include <QGLWidget>
#include <QSize>
#include <QtGui>

#include "Shader.hpp"

//
// Heavily borrowed from jakubcerveny/gl-compute
//
namespace qtwin
{
    class RenderWidget : public QGLWidget
    {
    Q_OBJECT

    public:
        RenderWidget(const QGLFormat& format);
        virtual ~RenderWidget();

    protected:

        virtual void initializeGL();
        virtual void resizeGL(int width, int height);
        virtual void paintGL();

        virtual void keyPressEvent(QKeyEvent* pEvent);

        QSize m_curSize;
        QSize m_texSize;

        Program m_progQuad;
        Program m_progCompute;

        GLuint m_vao;
        GLuint m_tex;

        double m_aspectRatio;
    };
}

