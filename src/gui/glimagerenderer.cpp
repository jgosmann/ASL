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


    //Image auf target zeichnen

    target->makeCurrent();
    m_textureID = target->bindTexture(*m_image);
    drawTexture();
    target->deleteTexture(m_textureID);
    target->doneCurrent();

    std::cout << m_shaderProgramList.size() << std::endl;

    //Shader anwenden

 if(m_useShaderProgram){

      QList<QSharedPointer<Shader> >::iterator shaderProgram;
      for(shaderProgram = m_shaderProgramList.begin();
        shaderProgram != m_shaderProgramList.end();
        shaderProgram++)
      {

        if(!(*shaderProgram)->isLinked())
          continue;

        std::swap(target,source);

         target->makeCurrent();

         GLuint texture = source->generateDynamicTexture();
         source->updateDynamicTexture(texture);

         glBindTexture(GL_TEXTURE_2D,texture);

         (*shaderProgram)->bind();
          drawTexture();
         (*shaderProgram)->release();

          target->deleteTexture(texture);

          target->doneCurrent();

      }

      }

//  m_sharedContext.deleteTexture(m_textureID);

//source->toImage().save("/home/denis/Programs/CPP/Computergrafik/cg/bin/test.jpg");

  emit framebufferObjectChanged(target);
}

void GLImageRenderer::drawTexture(){

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

}

//-- SLOTS --------------------------------------------------------------------

/** TODO: connect this one to your shaderList-Widget! */
void GLImageRenderer::renderImage(QList<QSharedPointer<Shader> > shaderProgramList)
{

    m_shaderProgramList = shaderProgramList;

  if(!source || !source->isValid() )
    return;

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

  if(source && source->isValid() )
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

  target->makeCurrent();

  glEnable(GL_TEXTURE_2D);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1.0, 1.0,-1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  target->doneCurrent();



  renderToFramebuffer();
}

void GLImageRenderer::saveImageFile()
{
  m_framebuffer->toImage().save( QFileDialog::getSaveFileName() );
}
