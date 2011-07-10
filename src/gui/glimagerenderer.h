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

#include "mainwindow.h"
#include "uniformsetter.h"

namespace gui {

typedef class asl::AnnotatedGLShaderProgram Shader;

class GLImageRenderer : public QObject
{
  Q_OBJECT

public:
    GLImageRenderer(const MainWindow *parent);
    ~GLImageRenderer();

    inline const QImage & getRenderedImage() const {
        return m_renderedImage;
    }

public slots:
    /* FIXME: Give this function a void argument list and introduce a setter
      * to set the shaderProgramList. This setter has to be called whenever
      * the program's shader list is changed to set it to the new list. (Do
      * not forget to call renderImage() afterwards. */
    void renderImage();
    void enableShaders(const int state);

    void setShaderProgramList(QList< QSharedPointer<Shader> > shaderProgramList);
    void setSourceImage(const QImage &img);

    inline void makeCurrent() { m_pixelBufferForContext.makeCurrent(); }
    inline void doneCurrent() { m_pixelBufferForContext.doneCurrent(); }

signals:
    void updated(const QImage &image);

private:
    UniformSetter *m_uniformSetter;

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
