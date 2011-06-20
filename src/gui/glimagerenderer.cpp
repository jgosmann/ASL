#include "glimagerenderer.h"

#include <iostream>

using namespace gui;

GLImageRenderer::GLImageRenderer(QObject* parent)
  : QObject(parent),
    m_useShaderProgram(false)
{
}

GLImageRenderer::~GLImageRenderer()
{
  delete m_openGLContext;
  delete m_framebuffer;
  delete m_image;
}

//-- MEMBER-FUNCTIONS ---------------------------------------------------------

void GLImageRenderer::renderToFramebuffer()
{
  if(!m_image)
    return;

  glDisable(GL_DEPTH_TEST);

  m_textureID = m_openGLContext->bindTexture(*m_image);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_framebuffer->bind();

  list<QGLShaderProgram*>::iterator shaderProgram;
  for(shaderProgram = m_shaderProgramList.begin();
    shaderProgram != m_shaderProgramList.end();
    shaderProgram++)
  {
    if(!(*shaderProgram)->isLinked())
      continue;

    if(m_useShaderProgram)
      (*shaderProgram)->bind();

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

    if(m_useShaderProgram)
      (*shaderProgram)->release();
  }
  m_framebuffer->release();

  m_openGLContext->deleteTexture(m_textureID);

  glEnable(GL_DEPTH_TEST);

  emit framebufferObjectChanged(m_framebuffer);
}

//-- SLOTS --------------------------------------------------------------------

void GLImageRenderer::setOpenGLContext(const QGLContext &context)
{
  if(!context.isValid())
    std::cout << "context is invalid!" << std::endl;
  m_openGLContext = &context;
}

/** TODO: connect this one to your shaderList-Widget! */
void GLImageRenderer::renderImage(list<QGLShaderProgram*> &shaderProgramList)
{
  if( !m_openGLContext->isValid() )
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

  if( m_openGLContext->isValid() )
    renderToFramebuffer();
}

void GLImageRenderer::loadImageFile()
{
  m_image = new QImage( QFileDialog::getOpenFileName() );

  m_openGLContext->makeCurrent();
  m_framebuffer = new QGLFramebufferObject( m_image->size() );
}

void GLImageRenderer::saveImageFile()
{
  m_framebuffer->toImage().save( QFileDialog::getSaveFileName() );
}
