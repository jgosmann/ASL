#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QGLContext>
#include <QGLFormat>
#include <QSharedPointer>
#include <QMessageBox>


#include <asl/annotatedglshaderprogram.h>
#include <asl/defaultaslprogramcompiler.h>

#include <iostream>

#include "glimagerenderer.h"

typedef class asl::AnnotatedGLShaderProgram Shader;

namespace Ui {
    class MainWindow;
}

namespace gui
{
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QGLContext *m_sharedContext;
    // this class connects the shaderList with the glImageViewer-widget
    GLImageRenderer *m_glImageRenderer;

    asl::DefaultASLProgramCompiler compiler;



public slots:
    void loadShaderDialog();
    void loadImageFile();
};
}

#endif // MAINWINDOW_H
