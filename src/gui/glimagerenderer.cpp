#include "glimagerenderer.h"

#include <cassert>
#include <iostream>

using namespace gui;

//FIXME
GLImageRenderer::GLImageRenderer()
    : QObject( NULL ), // parent ),
      m_uniformSetter( NULL ), // parent )
      m_useShaderProgram( true ),
      m_pixelBufferForContext( 1, 1 ),
      m_source( NULL ),
      m_target( NULL )
{
}

GLImageRenderer::~GLImageRenderer()
{
    if (m_source) {
        delete m_source;
    }
    if (m_target) {
        delete m_target;
    }
}

//-- MEMBER-FUNCTIONS ---------------------------------------------------------

void GLImageRenderer::render()
{
    if(!m_source || !m_target) {
        return;
    }

    m_pixelBufferForContext.makeCurrent();
    drawImageToTarget();

    if (m_useShaderProgram) {
        applyShaders();
    }

    m_renderedImage = m_target->toImage();
    m_pixelBufferForContext.doneCurrent();
    emit updated(m_renderedImage);
}

void GLImageRenderer::drawImageToTarget() {
    m_target->bind();
    const GLuint imgTex = m_pixelBufferForContext.bindTexture(m_sourceImage);
    drawTexture(imgTex);
    m_pixelBufferForContext.deleteTexture(imgTex);
    m_target->release();
}

void GLImageRenderer::applyShaders() {
    foreach (QSharedPointer<Shader> shaderProgram, m_shaderProgramList) {
        if(!shaderProgram->isLinked()) {
            continue;
        }

        std::swap(m_target, m_source);

        m_target->bind();
        GLuint sourceTexId = m_source->texture();
        shaderProgram->bind();
        shaderProgram->setUniformValue("tex", 0);
        shaderProgram->setUniformValue("texWidth", m_sourceImage.width());
        shaderProgram->setUniformValue("texHeight", m_sourceImage.height());

        // apply all custom uniforms
        m_uniformSetter->setUniforms( m_shaderProgramList.indexOf( 
              shaderProgram ) );

        drawTexture(sourceTexId);
        shaderProgram->release();
        m_target->release();
    }
}

void GLImageRenderer::drawTexture(GLuint tex){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1, -1);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1, -1);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1, 1);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1, 1);
    glEnd();
}

//-- SLOTS --------------------------------------------------------------------


void GLImageRenderer::setShaderProgramList(QList< QSharedPointer<Shader> > shaderProgramList)
{
  m_shaderProgramList = shaderProgramList;

  render();
}


void GLImageRenderer::enableShaders(const int state)
{
    if(((state == Qt::Checked) || (state == Qt::PartiallyChecked)))
    {
        m_useShaderProgram = true;
    } else {
        m_useShaderProgram = false;
    }

    render();
}


void GLImageRenderer::setSourceImage(const QImage &img)
{
    if (m_source) {
        delete m_source;
    }
    if (m_target) {
        delete m_target;
    }

#if __QT_VERSION__ >= 0x040700
    m_sourceImage.convertFromImage(img);
#else
    m_sourceImage = QPixmap::fromImage(img);
#endif

    m_pixelBufferForContext.makeCurrent();

    glViewport(0, 0, img.size().width(), img.size().height());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    m_source = new QGLFramebufferObject(m_sourceImage.size());
    m_target = new QGLFramebufferObject(m_sourceImage.size());

    m_pixelBufferForContext.doneCurrent();

    render();
}

