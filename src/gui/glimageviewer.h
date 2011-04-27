#ifndef GLIMAGEVIEWER_H
#define GLIMAGEVIEWER_H

#include <QGLShader>
#include <QGLShaderProgram>
#include <QGLWidget>
#include <QImage>
#include <QWidget>
#include <QRectF>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <QString>
#include <QGLFramebufferObject>
#include <iostream>

class GLImageViewer : public QGLWidget
{
    Q_OBJECT

public:
    GLImageViewer(QWidget *parent = NULL, Qt::WindowFlags f = 0);
    virtual ~GLImageViewer();

    void enableShaders(const int state);

public slots:
    void clearImage();
    void setImage(const QImage &image);

    void zoomImage(int value);
    void loadImageFile();
    void saveImageFile();

signals:
    void zoomChanged(int value);

protected:
    void virtual initializeGL();
    void virtual resizeGL(int w, int h);
    void virtual paintGL();

    void virtual keyPressEvent(QKeyEvent *event);
    void virtual wheelEvent(QWheelEvent *event);

private:
    bool m_textureLoaded;
    GLuint m_textureID;

    bool m_useShaderProgram;

    QImage m_image;
    float m_imageRatio;
    float m_imageZoom;
    QSize m_imageSize;
    QSize m_originalSize;
    QVector<QGLFramebufferObject*> m_frameBufferObjects;

    QGLShaderProgram m_shaderProgram;

    void calculateImageSize(int w, int h);
};

#endif // GLIMAGEVIEWER_H