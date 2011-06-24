#include "glimagerenderer.h"

#include <iostream>

using namespace gui;

GLImageRenderer::GLImageRenderer(QObject* parent, QGLContext &sharedContext)
  : QObject(parent),
    m_sharedContext(sharedContext),
    m_useShaderProgram(false),
    m_framebuffer(NULL),
    m_image(NULL)
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
  if(!m_image || !m_framebuffer)
    return;

  glDisable(GL_DEPTH_TEST);

  m_textureID = m_sharedContext.bindTexture(*m_image);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_framebuffer->bind();

  QList<QSharedPointer<Shader> >::iterator shaderProgram;
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

  m_sharedContext.deleteTexture(m_textureID);

  glEnable(GL_DEPTH_TEST);

  emit framebufferObjectChanged(m_framebuffer);
}

//-- SLOTS --------------------------------------------------------------------

/** TODO: connect this one to your shaderList-Widget! */
void GLImageRenderer::renderImage(QList<QSharedPointer<Shader> > &shaderProgramList)
{
  if( !m_sharedContext.isValid() )
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

  if( m_sharedContext.isValid() )
    renderToFramebuffer();
}

void GLImageRenderer::loadImageFile(QImage* img)
{
  m_image = img;

  m_sharedContext.makeCurrent();
  m_framebuffer = new QGLFramebufferObject( m_image->size() );
  renderToFramebuffer();
}

void GLImageRenderer::saveImageFile()
{
  m_framebuffer->toImage().save( QFileDialog::getSaveFileName() );
}
