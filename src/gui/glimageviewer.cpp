#include "glimageviewer.h"

#include <cassert>

using namespace gui;

GLImageViewer::GLImageViewer(QWidget *parent) : QGraphicsView(parent)
{
    setScene(&m_scene);
}


GLImageViewer::~GLImageViewer()
{
}


//-- SLOTS --------------------------------------------------------------------

void GLImageViewer::setImage(const QImage &img)
{
    m_scene.clear();
    m_scene.addPixmap(QPixmap::fromImage(img));
}

void GLImageViewer::setImageZoom(int value)
{
  resetMatrix();
  const qreal zoom = value / 100.0;
  scale(zoom, zoom);
}

//-- MEMBER - FUNCTIONS -------------------------------------------------------

//void GLImageViewer::keyPressEvent(QKeyEvent *event)
//{
  //switch(event->key())
  //{
  //case Qt::Key_Right:

      //updateGL();
      //break;
  //case Qt::Key_Left:

      //updateGL();
      //break;
  //case Qt::Key_Up:

      //updateGL();
      //break;
  //case Qt::Key_Down:

      //updateGL();
      //break;
  //}
//}

//void GLImageViewer::wheelEvent(QWheelEvent *event)
//{
    //if (event->delta() > 0)
        //m_imageZoom += 0.05;
    //else if (m_imageZoom >= 0.10)
        //m_imageZoom -= 0.05;

    //emit zoomChanged((int) (100 * m_imageZoom));
//}

