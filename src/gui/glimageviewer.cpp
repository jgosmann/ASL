#include "glimageviewer.h"

GLImageViewer::GLImageViewer(QWidget *parent, Qt::WindowFlags f) :
    QGLWidget(parent, NULL, f),
    m_textureLoaded(false),
    m_useShaderProgram(true),
    m_imageRatio(4.f/3.f),
    m_imageSize(QSize(1, 1)),
    m_originalSize(QSize(1, 1)),
    m_imageZoom(100)
{}

GLImageViewer::~GLImageViewer()
{
    clearImage();
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
    m_image = image;
    m_textureID = bindTexture(image);
    m_textureLoaded = true;

    m_imageRatio = (float) image.height() / image.width();
    m_imageSize = m_originalSize = image.size();
}

void GLImageViewer::zoomImage(int zoom)
{
    m_imageSize = m_originalSize * 0.01f * zoom;
    m_imageZoom = zoom;
    updateGL();
}

void GLImageViewer::initializeGL()
{
    glClearColor(0, 0, 0, 1);

    m_image = QImage("/home/mkastrop/Desktop/newyork.jpg");

    if (m_image.isNull()) {
        std::cerr << "load failed" << std::endl;
    } else {
        setImage(m_image);
    }

//    QGLShader lowKey(QGLShader::Fragment);
//    lowKey.compileSourceCode(
//            "uniform sampler2D texture;\n"
//            "float len;\n"
//            "void lowKey(void)\n"
//            "{\n"
//            "   len = length(texture2D(texture, gl_TexCoord[0].xy).rgb);\n"
//            "   if (len < 0.1) {\n"
//            "       gl_FragColor = vec4(1, 0, 0, 1);\n"
//            "   }\n"
//            "}");

//    QGLShader highKey(QGLShader::Fragment);
//    highKey.compileSourceCode(
//            "uniform sampler2D texture;\n"
//            "float len;\n"
//            "void highKey(void)\n"
//            "{\n"
//            "   len = length(texture2D(texture, gl_TexCoord[0].xy).rgb);\n"
//            "   if (len > 0.9) {\n"
//            "       gl_FragColor = vec4(0, 1, 0, 1);\n"
//            "   }\n"
//            "}");

    QGLShader main(QGLShader::Fragment);
    main.compileSourceCode(
            "uniform sampler2D texture;\n"
            "//void lowKey(void);\n"
            "//void highKey(void);\n"
            "float lighting;\n"
            "void main(void)\n"
            "{\n"
            "   gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);\n"
            "   lighting = length(gl_FragColor.rgb);\n"
            "   gl_FragColor = lighting * vec4(1, 1, 1, 1.0/lighting);\n"
            "   //lowKey();\n"
            "   //highKey();\n"
            "}");

    m_shaderProgram.addShader(&main);
    //shaderProgram.addShader(&lowKey);
    //shaderProgram.addShader(&highKey);

    m_shaderProgram.link();
}

void GLImageViewer::resizeGL(int w, int h)
{
    calculateImageSize(w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat) w / h, 0.1f, 100.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLImageViewer::calculateImageSize(int w, int h)
{
    float diff_x = w - m_imageSize.width();
    float diff_y = h - m_imageSize.height();

    if (diff_x < 0 && diff_y < 0) glViewport(0, diff_y, m_imageSize.width(), m_imageSize.height());
    if (diff_x >= 0 && diff_y < 0) glViewport(diff_x / 2, diff_y, m_imageSize.width(), m_imageSize.height());
    if (diff_x < 0 && diff_y >= 0) glViewport(0, diff_y / 2, m_imageSize.width(), m_imageSize.height());
    if (diff_x >= 0 && diff_y >= 0) glViewport(diff_x / 2, diff_y / 2, m_imageSize.width(), m_imageSize.height());
}

void GLImageViewer::paintGL()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glClear(GL_COLOR_BUFFER_BIT);

    calculateImageSize(width(), height());

    // switch to ortho
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width(), 0, m_imageRatio * width(), 0.f, 1000.f);
    // ---------------

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (m_useShaderProgram) m_shaderProgram.bind();

    bindTexture(m_image);

    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f,  0.0f, -1.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(width(),  0.0f, -1.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(width(), m_imageRatio * width(), -1.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0.0f, m_imageRatio * width(), -1.0f);
    glEnd();

    if (m_useShaderProgram) m_shaderProgram.release();

    // switch to frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(0.f, width(), 0.f, m_imageRatio * width(), 0.f, 1000.f);
    // ---------------

    glEnable(GL_DEPTH_TEST);
}

void GLImageViewer::enableShaders(const int _state)
{
    if((_state == Qt::Checked) || (_state == Qt::PartiallyChecked)) {
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
    int value = m_imageZoom + (int) (event->delta() * .042f);

    if (value >= 5) {
        zoomImage(value);
        emit zoomChanged(value);
    }
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

    grabFrameBuffer().copy(diff_x, diff_y, width()-2*diff_x, height()-2*diff_y).save(filename);
}
