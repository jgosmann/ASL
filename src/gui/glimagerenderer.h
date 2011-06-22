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
    GLImageRenderer(QObject *parent, QGLContext &sharedContext);
    ~GLImageRenderer();

  public slots:
    void renderImage(list<QGLShaderProgram*> &shaderProgramList);
    void enableShaders(const int state);
    void loadImageFile();
    void saveImageFile();

  signals:
    void framebufferObjectChanged(QGLFramebufferObject *framebuffer);

  private:
    void renderToFramebuffer();

    bool m_useShaderProgram;

    QGLContext &m_sharedContext;

    QGLFramebufferObject *m_framebuffer;
    list<QGLShaderProgram*> m_shaderProgramList;

    QImage *m_image;
    GLuint m_textureID;
  };
}

#endif // GLIMAGERENDERER_H
