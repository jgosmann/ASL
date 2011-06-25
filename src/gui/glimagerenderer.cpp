#include "glimagerenderer.h"

#include <iostream>

using namespace gui;

GLImageRenderer::GLImageRenderer(QObject* parent)
  : QObject(parent),
    m_useShaderProgram(false),
    m_framebuffer(NULL),
    m_image(NULL),
    source(NULL),
    target(NULL)
{
}

GLImageRenderer::~GLImageRenderer()
{
  if(m_framebuffer)
    delete m_framebuffer;
  if(m_image)
    delete m_image;
}

//-- MEMBER-FUNCTIONS ---------------------------------------------------------

void GLImageRenderer::renderToFramebuffer()
{
  //TODO:
  if(!m_image || !source || !target)
    return;



    bool established = source->makeCurrent();
//  m_textureID = m_sharedContext.bindTexture(*m_image);
    m_textureID = source->bindTexture(*m_image);

//    glDisable(GL_DEPTH_TEST);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



//  m_framebuffer->bind();

//  QList<QSharedPointer<Shader> >::iterator shaderProgram;
//  for(shaderProgram = m_shaderProgramList.begin();
//    shaderProgram != m_shaderProgramList.end();
//    shaderProgram++)
//  {
//    if(!(*shaderProgram)->isLinked())
//      continue;

//    if(m_useShaderProgram)
//      (*shaderProgram)->bind();

    glBegin(GL_QUADS);
      glNormal3f( 0.0f, 0.0f, 1.0f);

      glTexCoord2f( 0.0f, 0.0f);
      glVertex2f(-1.0f, 1.0f);

      glTexCoord2f( 1.0f, 0.0f);
      glVertex2f( 1.0f, 1.0f);

      glTexCoord2f( 1.0f, 1.0f);
      glVertex2f( 1.0f,-1.0f);

      glTexCoord2f( 0.0f, 1.0f);
      glVertex2f(-1.0f,-1.0f);
    glEnd();

    glEnable(GL_DEPTH_TEST);

    source->deleteTexture(m_textureID);


//    source->drawTexture(QRect(QPoint(0,0),m_image->size()),m_textureID);
    source->doneCurrent();

//    if(m_useShaderProgram)
//      (*shaderProgram)->release();
//  }
//  m_framebuffer->release();

//  m_sharedContext.deleteTexture(m_textureID);

//source->toImage().save("/home/denis/Programs/CPP/Computergrafik/cg/bin/test.jpg");

  emit framebufferObjectChanged(source);
}

//-- SLOTS --------------------------------------------------------------------

/** TODO: connect this one to your shaderList-Widget! */
void GLImageRenderer::renderImage(QList<QSharedPointer<Shader> > &shaderProgramList)
{
  if( !source->isValid() )
    return;

  m_shaderProgramList = shaderProgramList;
  renderToFramebuffer();
}

void GLImageRenderer::enableShaders(const int state)
{
  if(((state == Qt::Checked) || (state == Qt::PartiallyChecked)))
  {
    m_useShaderProgram = true;
  } else {
    m_useShaderProgram = false;
  }

  if( source->isValid() )
    renderToFramebuffer();
}

void GLImageRenderer::loadImageFile(QImage* img)
{
  m_image = img;

//  m_sharedContext.makeCurrent();
//  m_framebuffer = new QGLFramebufferObject( m_image->size() );
  source = new QGLPixelBuffer(img->size());

  source->makeCurrent();
  glEnable(GL_TEXTURE_2D);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
//  glOrtho(-1.0, +1.0, -1.0, +1.0, -90.0, +90.0);
  gluOrtho2D(-1.0, 1.0,-1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  source->doneCurrent();

  target = new QGLPixelBuffer(img->size());
  renderToFramebuffer();
}

void GLImageRenderer::saveImageFile()
{
  m_framebuffer->toImage().save( QFileDialog::getSaveFileName() );
}
