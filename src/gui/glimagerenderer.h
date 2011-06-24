#ifndef GLIMAGERENDERER_H
#define GLIMAGERENDERER_H

#include <QObject>
#include <QGLContext>
#include <QGLFramebufferObject>
#include <QGLShaderProgram>
#include <QFileDialog>
#include <QList>
#include <QSharedPointer>
#include <asl/annotatedglshaderprogram.h>
#include <QGLPixelBuffer>

namespace gui {


  typedef class asl::AnnotatedGLShaderProgram Shader;

  class GLImageRenderer : public QObject
  {
    Q_OBJECT

  public:
    GLImageRenderer(QObject *parent, QGLContext &sharedContext);
    ~GLImageRenderer();

  public slots:
    void renderImage(QList<QSharedPointer<Shader> > &shaderProgramList);
    void enableShaders(const int state);
    void loadImageFile(QImage* img);
    void saveImageFile();

  signals:
    void framebufferObjectChanged(QGLPixelBuffer *framebuffer);

  private:
    void renderToFramebuffer();

    bool m_useShaderProgram;

    QGLContext &m_sharedContext;

    QGLFramebufferObject *m_framebuffer;
    QList<QSharedPointer<Shader> > m_shaderProgramList;

    QGLPixelBuffer* target;
    QGLPixelBuffer* source;

    QImage *m_image;
    GLuint m_textureID;
  };
}

#endif // GLIMAGERENDERER_H
