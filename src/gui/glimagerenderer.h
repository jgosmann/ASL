#ifndef GLIMAGERENDERER_H
#define GLIMAGERENDERER_H

#include <QObject>
#include <QGLContext>
#include <QGLFramebufferObject>
#include <QGLShaderProgram>
#include <QFileDialog>

namespace gui {

  using std::list;

  class GLImageRenderer : public QObject
  {
    Q_OBJECT

  public:
    GLImageRenderer(QObject *parent);
    ~GLImageRenderer();

  public slots:
    inline void setOpenGLContext(const QGLContext &context);
    inline void renderImage(list<QGLShaderProgram*> &shaderProgramList);
    inline void enableShaders(const int state);
    inline void loadImageFile();
    inline void saveImageFile();

  signals:
    void framebufferObjectChanged(QGLFramebufferObject *framebuffer);

  private:
    void renderToFramebuffer();

    bool m_useShaderProgram;

    QGLContext *m_openGLContext;

    QGLFramebufferObject *m_framebuffer;
    list<QGLShaderProgram*> m_shaderProgramList;

    QImage *m_image;
    GLuint m_textureID;
  };
}

#endif // GLIMAGERENDERER_H
