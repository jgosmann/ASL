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
    m_renderedImage = m_renderBuffer->toImage().copy(0, 0,
            m_sourceImage.width(), m_sourceImage.height());
    m_renderedImage.save("/Users/blubb/Desktop/test2.png");
    emit updated(m_renderedImage);
}

void GLImageRenderer::drawImageToRenderBuffer() {
    const GLuint imgTex = m_renderBuffer->bindTexture(m_sourceImage);
    drawTexture(imgTex);
    m_renderBuffer->deleteTexture(imgTex);
}

void GLImageRenderer::applyShaders() {
    GLuint source = m_renderBuffer->generateDynamicTexture();

    foreach (QSharedPointer<Shader> shaderProgram, m_shaderProgramList) {
        if(!shaderProgram->isLinked()) {
            continue;
        }

        m_renderBuffer->updateDynamicTexture(source);
        shaderProgram->bind();
        shaderProgram->setUniformValue("tex", source);
        shaderProgram->setUniformValue("texWidth", m_sourceImage.width());
        shaderProgram->setUniformValue("texHeight", m_sourceImage.height());
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
        delete m_renderBuffer;
    }

    m_sourceImage.convertFromImage(img);

    const unsigned int width = nextPowerOf2GTE(m_sourceImage.width());
    const unsigned int height = nextPowerOf2GTE(m_sourceImage.height());
    if (m_shareWidget) {
        m_renderBuffer = new QGLPixelBuffer(width, height,
                m_shareWidget->format(), m_shareWidget);
    } else {
        m_renderBuffer = new QGLPixelBuffer(width, height);
    }

    render();
}

inline quint32 GLImageRenderer::nextPowerOf2GTE(register quint32 x) {
    /* Algorithm based on 
     * <http://aggregate.org/MAGIC/#Next%20Largest%20Power%20of%202> */
    --x;
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return x + 1;
}

