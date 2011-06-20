#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QGLContext>
#include <QGLFormat>

#include <iostream>

#include "glimagerenderer.h"

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

    // this class connects the shaderList with the glImageViewer-widget
    GLImageRenderer *m_glImageRenderer;
};
}

#endif // MAINWINDOW_H
