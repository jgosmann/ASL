#ifndef GLIMAGERENDERER_H
#define GLIMAGERENDERER_H

#include <QObject>
#include <QGLContext>
#include <QGLShaderProgram>
#include <QGLWidget>
#include <QFileDialog>
#include <QList>
#include <QSharedPointer>
#include <asl/annotatedglshaderprogram.h>
#include <QGLFramebufferObject>
#include <QGLPixelBuffer>
#include <algorithm>
#include <iostream>

namespace gui {

typedef class asl::AnnotatedGLShaderProgram Shader;

class GLImageRenderer : public QObject
{
    Q_OBJECT

    public:
        GLImageRenderer(QObject *parent);
        ~GLImageRenderer();

        inline const QImage & getRenderedImage() const {
            return m_renderedImage;
        }

    public slots:
        void renderImage(QList<QSharedPointer<Shader> > shaderProgramList);
        void enableShaders(const int state);
        void setSourceImage(const QImage &img);

        inline void makeCurrent() { m_pixelBufferForContext.makeCurrent(); }
        inline void doneCurrent() { m_pixelBufferForContext.doneCurrent(); }

    signals:
        void updated(const QImage &image);

    private:
        void render();
        void drawImageToTarget();
        void applyShaders();
        void drawTexture(GLuint tex);

        bool m_useShaderProgram;

        QList<QSharedPointer<Shader> > m_shaderProgramList;

        QGLPixelBuffer m_pixelBufferForContext;
        QPixmap m_sourceImage;
        QImage m_renderedImage;
        QGLFramebufferObject *m_source;
        QGLFramebufferObject *m_target;
};
}

#endif // GLIMAGERENDERER_H
