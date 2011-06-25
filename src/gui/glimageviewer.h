#ifndef GLIMAGEVIEWER_H
#define GLIMAGEVIEWER_H

#include <QGLContext>
#include <QGLWidget>
#include <QImage>
#include <QWidget>
#include <QRectF>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <QString>
#include <QGLFramebufferObject>

#include "mainwindow.h"

namespace gui {

  class GLImageViewer : public QGLWidget
  {
      Q_OBJECT

  public:
      GLImageViewer(QWidget *parent=NULL, Qt::WindowFlags f=0);
      virtual ~GLImageViewer();

  public slots:
    void updateFramebufferObject(QGLPixelBuffer *framebuffer);
    void setImageZoom(int value);

  signals:
    void zoomChanged(int value);

  protected:
    void virtual initializeGL();
    void virtual resizeGL(int w, int h);
    void virtual paintGL();

    void virtual keyPressEvent(QKeyEvent *event);
    void virtual wheelEvent(QWheelEvent *event);

  private:
    QGLContext *m_openGLContext;
    QGLPixelBuffer *m_framebuffer;
    QImage m_image;

    float m_imageRatio;
    float m_imageZoom;
  };

}

#endif // GLIMAGEVIEWER_H
