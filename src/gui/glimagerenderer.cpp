#include "glimagerenderer.h"

#include <cassert>
#include <iostream>

using namespace gui;

GLImageRenderer::GLImageRenderer(QObject* parent, QGLWidget *shareWidget)
    : QObject(parent),
    m_useShaderProgram(true),
    m_shareWidget(shareWidget),
    m_renderBuffer(NULL)
{
}

GLImageRenderer::~GLImageRenderer()
{
    if(m_renderBuffer) {
        delete m_source;
        delete m_target;
        delete m_renderBuffer;
    }
}

//-- MEMBER-FUNCTIONS ---------------------------------------------------------

void GLImageRenderer::render()
{
    if(!m_renderBuffer) {
        return;
    }

    m_renderBuffer->makeCurrent();
    drawImageToTarget();

    if (m_useShaderProgram) {
        applyShaders();
    }

    m_renderedImage = m_target->toImage();
    m_renderBuffer->doneCurrent();
    emit updated(m_renderedImage);
}

void GLImageRenderer::drawImageToTarget() {
    m_target->bind();
    const GLuint imgTex = m_renderBuffer->bindTexture(m_sourceImage);
    drawTexture(imgTex);
    m_renderBuffer->deleteTexture(imgTex);
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
        shaderProgram->setUniformValue("tex", sourceTexId);
        shaderProgram->setUniformValue("texWidth", m_sourceImage.width());
        shaderProgram->setUniformValue("texHeight", m_sourceImage.height());
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

    const GLfloat w = 2.0 * m_sourceImage.width() / m_renderBuffer->width();
    const GLfloat h = 2.0 * m_sourceImage.height() / m_renderBuffer->height();

    glLoadIdentity();
    glTranslatef(-1, 1 - h, 0);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0, 0);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(w, 0);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(w, h);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0, h);
    glEnd();
}

//-- SLOTS --------------------------------------------------------------------

/** TODO: connect this one to your shaderList-Widget! */
void GLImageRenderer::renderImage(QList<QSharedPointer<Shader> > shaderProgramList)
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
    if (m_renderBuffer) {
        delete m_source;
        delete m_target;
        delete m_renderBuffer;
    }

    m_sourceImage.convertFromImage(img);

    if (m_shareWidget) {
        m_renderBuffer = new QGLPixelBuffer(m_sourceImage.size(),
                m_shareWidget->format(), m_shareWidget);
    } else {
        m_renderBuffer = new QGLPixelBuffer(m_sourceImage.size());
    }

    m_renderBuffer->makeCurrent();
    m_source = new QGLFramebufferObject(m_sourceImage.size());
    m_target = new QGLFramebufferObject(m_sourceImage.size());
    m_renderBuffer->doneCurrent();

    render();
}

