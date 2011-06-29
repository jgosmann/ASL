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
#include <QGLPixelBuffer>
#include <algorithm>
#include <iostream>

namespace gui {

typedef class asl::AnnotatedGLShaderProgram Shader;

class GLImageRenderer : public QObject
{
    Q_OBJECT

    public:
        GLImageRenderer(QObject *parent, QGLWidget *shareWidget = NULL);
        ~GLImageRenderer();

        inline const QImage & getRenderedImage() const {
            return m_renderedImage;
        }

    public slots:
        void renderImage(QList<QSharedPointer<Shader> > shaderProgramList);
        void enableShaders(const int state);
        void setSourceImage(const QImage &img);

    signals:
        void updated(const QImage &image);

    private:
        void render();
        void drawImageToRenderBuffer();
        void applyShaders();
        void drawTexture(GLuint tex);

        bool m_useShaderProgram;

        QList<QSharedPointer<Shader> > m_shaderProgramList;

        QGLWidget *m_shareWidget;
        QPixmap m_sourceImage;
        QImage m_renderedImage;
        QGLPixelBuffer* m_renderBuffer;
};
}

#endif // GLIMAGERENDERER_H
