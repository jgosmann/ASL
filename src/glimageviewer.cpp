#include "glimageviewer.h"

#include <QGLShader>
#include <QRectF>
#include <iostream>

GLImageViewer::GLImageViewer(QWidget *parent, Qt::WindowFlags f) :
    QGLWidget(parent, NULL, f),
    textureLoaded(false)
{
}

GLImageViewer::~GLImageViewer()
{
    clearImage();
}

void GLImageViewer::clearImage()
{
    if (textureLoaded) {
        deleteTexture(textureId);
        textureLoaded = false;
    }
}

void GLImageViewer::setImage(const QImage &image)
{
    textureId = bindTexture(image);
    textureLoaded = true;
}

void GLImageViewer::initializeGL()
{
    glClearColor(0, 0, 0, 0);

    QImage image("/Volumes/Home/blubb/Documents/programming/uni/cg/imgs/test.jpg");
    if (image.isNull()) {
        std::cerr << "load failed" << std::endl;
    } else {
        setImage(image);
    }

    QGLShader lowKey(QGLShader::Fragment);
    lowKey.compileSourceCode(
            "uniform sampler2D texture;\n"
            "float len;\n"
            "void lowKey(void)\n"
            "{\n"
            "   len = length(texture2D(texture, gl_TexCoord[0].xy).rgb);\n"
            "   if (len < 0.1) {\n"
            "       gl_FragColor = vec4(1, 0, 0, 1);\n"
            "   }\n"
            "}");

    QGLShader highKey(QGLShader::Fragment);
    highKey.compileSourceCode(
            "uniform sampler2D texture;\n"
            "float len;\n"
            "void highKey(void)\n"
            "{\n"
            "   len = length(texture2D(texture, gl_TexCoord[0].xy).rgb);\n"
            "   if (len > 0.9) {\n"
            "       gl_FragColor = vec4(0, 1, 0, 1);\n"
            "   }\n"
            "}");

    QGLShader main(QGLShader::Fragment);
    main.compileSourceCode(
            "uniform sampler2D texture;\n"
            "void lowKey(void);\n"
            "void highKey(void);\n"
            "void main(void)\n"
            "{\n"
            "   gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);\n"
            "   lowKey();\n"
            "   highKey();\n"
            "}");

    shaderProgram.addShader(&main);
    shaderProgram.addShader(&lowKey);
    shaderProgram.addShader(&highKey);

    shaderProgram.link();
}

void GLImageViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram.bind();

    if (textureLoaded) {
        drawTexture(QRectF(-1, -1, 2, 2), textureId);
    }

    shaderProgram.release();
}
