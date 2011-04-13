#include "glimageviewer.h"

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
}

void GLImageViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (textureLoaded) {
        drawTexture(QRectF(-1, -1, 2, 2), textureId);
    }
}
