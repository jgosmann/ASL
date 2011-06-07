#ifndef GLIMAGEVIEWER_H
#define GLIMAGEVIEWER_H

#include <QGLShader>
#include <QGLShaderProgram>
#include <QGLWidget>
#include <QImage>
#include <QWidget>
#include <QRectF>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <QString>
#include <QGLFramebufferObject>
#include <iostream>

#include "asl/gltypeinfo.h"

namespace gui {

  /**
  *  This class loads an abitrary image-file to the framebuffer and applies all
  *  shaders in the current shaderProgram on it.
  */
  class GLImageViewer : public QGLWidget
  {
      Q_OBJECT

  public:
      GLImageViewer(QWidget *parent = NULL, Qt::WindowFlags f = 0);
      virtual ~GLImageViewer();

      void enableShaders(const int state);


  public slots:
      void clearImage();
      void setImage(const QImage &image);

      void loadImageFile();
      void saveImageFile();

      void setImageZoom(int value);

      /**
      *  This slot allows other widgets who are capable of loading shader files
      *  to add them to the GLImageViewer widget.
      */
      void addShader(QGLShader& shader);

      /**
       *  This slot receives all information needed to execute "setUniform" on
       *  m_shaderProgram when the user has changed any of the uniforms in the
       *  GUI.
       */
      void applyUniform(const QString& glslName, const void* value);

  signals:
      void zoomChanged(int value);

  protected:
      void virtual initializeGL();
      void virtual resizeGL(int w, int h);
      void virtual paintGL();

      void virtual keyPressEvent(QKeyEvent *event);
      void virtual wheelEvent(QWheelEvent *event);

  private:
      void renderToFramebuffer();

      bool m_textureLoaded;
      GLuint m_textureID;

      bool m_useShaderProgram;

      QImage *m_image;
      float m_imageRatio;
      float m_imageZoom;
      QSize m_imageSize;
      QSize m_originalSize;
      QGLFramebufferObject *m_frameBuffer;

      QGLShaderProgram *m_shaderProgram;
  };

}

#endif // GLIMAGEVIEWER_H
