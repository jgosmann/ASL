#ifndef GLIMAGEVIEWER_H
#define GLIMAGEVIEWER_H

#include <QGLShader>
#include <QGLShaderProgram>
#include <QGLWidget>
#include <QImage>
#include <QWidget>

class GLImageViewer : public QGLWidget
{
    Q_OBJECT

public:
    GLImageViewer(QWidget *parent = NULL, Qt::WindowFlags f = 0);
    virtual ~GLImageViewer();

public slots:
    void clearImage();
    void setImage(const QImage &image);

protected:
    void virtual initializeGL();
    void virtual paintGL();

private:
    bool textureLoaded;
    GLuint textureId;

    QGLShaderProgram shaderProgram;
};

#endif // GLIMAGEVIEWER_H
