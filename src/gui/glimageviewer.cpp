#include "glimageviewer.h"

#include <assert.h>

using namespace gui;

GLImageViewer::GLImageViewer(QWidget *parent, Qt::WindowFlags f) :
    QGLWidget(parent, NULL, f),
    m_imageZoom(1.0f),
    m_framebuffer(NULL)
{
}


GLImageViewer::~GLImageViewer()
{
  if(m_framebuffer)
    delete m_framebuffer;
}

//-- OpenGL core functions ----------------------------------------------------

void GLImageViewer::initializeGL()
{
  glEnable(GL_TEXTURE_2D);
}

void GLImageViewer::resizeGL(int w, int h)
{
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
  if(!m_framebuffer)
    return;

  makeCurrent();

  glDisable(GL_DEPTH_TEST);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // load & set initial default image
  GLuint texture = bindTexture( m_framebuffer->toImage() );

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

  deleteTexture( texture );


  glEnable(GL_DEPTH_TEST);
}

//-- SLOTS --------------------------------------------------------------------

void GLImageViewer::updateFramebufferObject(QGLPixelBuffer *framebuffer)
{
  m_framebuffer = framebuffer;
  m_imageRatio = (float) m_framebuffer->toImage().width() / 
    m_framebuffer->toImage().height();
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
