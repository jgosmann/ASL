#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QCoreApplication>
#include <QScrollArea>
#include <QGLContext>
#include <QGLFormat>
#include <QSharedPointer>
#include <QMessageBox>


#include <asl/annotatedglshaderprogram.h>
#include <asl/defaultaslprogramcompiler.h>
#include <asl/gltypeinfo.h>

#include <iostream>

#include "glimagerenderer.h"
#include "shaderparametercontrol.h"

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

public slots:
    void loadShaderDialog();
    void loadImageFile();
    void saveImage();
    void showControls(QSharedPointer<Shader>);

signals:
    void exitProgram();

private:
    Ui::MainWindow *ui;

    QGLContext *m_sharedContext;
    // this class connects the shaderList with the glImageViewer-widget
    GLImageRenderer *m_glImageRenderer;

    asl::DefaultASLProgramCompiler compiler;

    QList<int> m_shaderProgramIDs;

private slots:
    void emitExit();
};
}

#endif // MAINWINDOW_H
