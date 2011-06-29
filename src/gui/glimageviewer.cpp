#include "glimageviewer.h"

#include <assert.h>

using namespace gui;

GLImageViewer::GLImageViewer(QWidget *parent, Qt::WindowFlags f) :
    QGLWidget(parent, NULL, f),
    m_image(NULL),
    m_imageZoom(1.0f)
{
}


GLImageViewer::~GLImageViewer()
{
}

//-- OpenGL core functions ----------------------------------------------------

void GLImageViewer::initializeGL()
{
  glEnable(GL_TEXTURE_2D);
}

void GLImageViewer::resizeGL(int w, int h)
{
  makeCurrent();
  // set the viewpoint dependent to image size
  glViewport(0, 0, w, h);//m_image->width(), m_image->height());

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(-1.0, 1.0,-1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void GLImageViewer::paintGL()
{
  if(m_image == 0)
    return;

  glEnable(GL_TEXTURE_2D);

  glDisable(GL_DEPTH_TEST);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  bindTexture(*m_image);

  glLoadIdentity();
  glScalef(m_imageZoom, m_imageZoom, 1.0f);

  glBegin(GL_QUADS);
      glNormal3f( 0.0f, 0.0f, 1.0f);

      glTexCoord2f( 0.0f, 0.0f);
      glVertex2f(-1.0f, m_imageRatio);

      glTexCoord2f( 1.0f, 0.0f);
      glVertex2f( 1.0f, m_imageRatio);

      glTexCoord2f( 1.0f, 1.0f);
      glVertex2f( 1.0f,-m_imageRatio);

      glTexCoord2f( 0.0f, 1.0f);
      glVertex2f(-1.0f,-m_imageRatio);
  glEnd();

  glEnable(GL_DEPTH_TEST);
}

//-- SLOTS --------------------------------------------------------------------

void GLImageViewer::setImage(const QImage &img)
{
  m_image = &img;
  m_imageRatio = 1.3; 
  // FIXME set ratio to correct value, glGetTexLevelParameter can get the
  // texture size.
  glDraw();

}

void GLImageViewer::setImageZoom(int value)
{
  m_imageZoom = (float) value / 100;
  updateGL();
}

//-- MEMBER - FUNCTIONS -------------------------------------------------------

void GLImageViewer::keyPressEvent(QKeyEvent *event)
{
  switch(event->key())
  {
  case Qt::Key_Right:

      updateGL();
      break;
  case Qt::Key_Left:

      updateGL();
      break;
  case Qt::Key_Up:

      updateGL();
      break;
  case Qt::Key_Down:

      updateGL();
      break;
  }
}

void GLImageViewer::wheelEvent(QWheelEvent *event)
{
  if (event->delta() > 0)
    m_imageZoom += 0.05;
  else if (m_imageZoom >= 0.10)
    m_imageZoom -= 0.05;

  emit zoomChanged((int) (100 * m_imageZoom));

  updateGL();
}
