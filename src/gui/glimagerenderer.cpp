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
    drawImageToRenderBuffer();

    if (m_useShaderProgram) {
        applyShaders();
    }

    m_renderBuffer->doneCurrent();
    emit updated(m_renderBuffer->toImage());
}

void GLImageRenderer::drawImageToRenderBuffer() {
    const GLuint imgTex = m_renderBuffer->bindTexture(m_sourceImage);
    drawTexture(imgTex);
    m_renderBuffer->deleteTexture(imgTex);
    m_renderBuffer->toImage().save("/Users/blubb/Desktop/test2.png");
}

void GLImageRenderer::applyShaders() {
    GLuint source = m_renderBuffer->generateDynamicTexture();

    foreach (QSharedPointer<Shader> shaderProgram, m_shaderProgramList) {
        if(!shaderProgram->isLinked()) {
            continue;
        }

        m_renderBuffer->updateDynamicTexture(source);
        shaderProgram->bind();
        drawTexture(source);
        shaderProgram->release();
    }

    m_renderBuffer->deleteTexture(source);
}

void GLImageRenderer::drawTexture(GLuint tex){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

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
        delete m_renderBuffer;
    }

    m_sourceImage.convertFromImage(img);

    // FIXME use powers of 2
    m_renderBuffer = new QGLPixelBuffer(512, 512, m_shareWidget->format(), m_shareWidget);

    render();
}

