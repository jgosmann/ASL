#include "glimageviewer.h"

using namespace gui;

GLImageViewer::GLImageViewer(QWidget *parent, Qt::WindowFlags f) :
    QGLWidget(parent, NULL, f),
    m_textureLoaded(false),
    m_useShaderProgram(true),
    m_imageZoom(1.0f)
{
}

GLImageViewer::~GLImageViewer()
{
    clearImage();
    delete m_image;
    m_image = 0;
    delete m_frameBuffer;
    m_frameBuffer = 0;
}

void GLImageViewer::initializeGL()
{
    glEnable(GL_TEXTURE_2D);

    // load & set initial default image
    m_image = new QImage("./data/intro.jpg");
    setImage(*m_image);

    // set the viewpoint dependent to image size
    glViewport(0, 0, m_image->width(), m_image->height());

    QGLShader main(QGLShader::Fragment);
    main.compileSourceCode(
            "uniform sampler2D texture;\n"
            "float lighting;\n"
            "void main(void)\n"
            "{\n"
            "   gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);\n"
            "   lighting = length(gl_FragColor.rgb);\n"
            "   gl_FragColor = lighting * vec4(1, 1, 1, 1.0/lighting);\n"
            "   //lowKey();\n"
            "   //highKey();\n"
            "}"
    );

    m_shaderProgram.addShader(&main);

    m_shaderProgram.link();

    bindTexture(*m_image);
    makeCurrent();
    m_frameBuffer = new QGLFramebufferObject(m_image->size());
    renderToFramebuffer();
    m_frameBuffer->toImage().save("/home/mkastrop/Desktop/framebuffer.png");
    bindTexture(m_frameBuffer->toImage());
}

void GLImageViewer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-1.0, 1.0,-1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLImageViewer::paintGL()
{
    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

void GLImageViewer::clearImage()
{
    if (m_textureLoaded) {
        deleteTexture(m_textureID);
        m_textureLoaded = false;
    }
}

void GLImageViewer::setImage(const QImage &image)
{
    *m_image = image;
    m_textureID = bindTexture(*m_image);
    m_textureLoaded = true;

    m_imageRatio = (float) m_image->height() / m_image->width();
    m_imageSize = m_originalSize = m_image->size();
}

void GLImageViewer::renderToFramebuffer()
{
    glDisable(GL_DEPTH_TEST);

    bindTexture(*m_image);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_frameBuffer->bind();

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

    m_frameBuffer->release();

    glEnable(GL_DEPTH_TEST);
}

void GLImageViewer::enableShaders(const int state)
{
    if((state == Qt::Checked) || (state == Qt::PartiallyChecked)) {
        m_useShaderProgram = true;
    } else {
        m_useShaderProgram = false;
    }
    updateGL();
}

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

void GLImageViewer::loadImageFile()
{
    QString filename = QFileDialog::getOpenFileName();

    setImage(QImage(filename));

    updateGL();
}

void GLImageViewer::saveImageFile()
{
    QString filename = QFileDialog::getSaveFileName();

    float diff_x = width() - m_imageSize.width();
    float diff_y = height() - m_imageSize.height();

    m_frameBuffer->toImage().save(filename);
    //grabFrameBuffer().copy(diff_x, diff_y, width()-2*diff_x, height()-2*diff_y).save(filename);
}

void GLImageViewer::setImageZoom(int value)
{
    m_imageZoom = (float) value / 100;
    updateGL();
}
