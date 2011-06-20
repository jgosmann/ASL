#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace gui;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set working directory to execution directory
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    m_glImageRenderer = new GLImageRenderer(this);

    connect(ui->spinBox_Zoom, SIGNAL(valueChanged(int)), 
            ui->glDisplay, SLOT(setImageZoom(int)));
    connect(ui->glDisplay, SIGNAL(zoomChanged(int)), 
            ui->spinBox_Zoom, SLOT(setValue(int)));

    connect(ui->glDisplay, SIGNAL(
              shareOpenGLContext(QGLContext&)),
            m_glImageRenderer, SLOT(
              setOpenGLContext(QGLContext&)));

    connect(ui->action_LoadImage, SIGNAL(changed()), 
            m_glImageRenderer, SLOT(loadImageFile()));
    connect(ui->action_SaveImage, SIGNAL(changed()), 
            m_glImageRenderer, SLOT(saveImageFile()));
    connect(ui->checkBox_ApplyShaders, SIGNAL(stateChanged(int)), 
            m_glImageRenderer, SLOT(enableShaders(int)));
    connect(m_glImageRenderer, SIGNAL(framebufferObjectChanged(
              QGLFramebufferObject&)),
            ui->glDisplay, SLOT(updateFramebufferObject(
              QGLFramebufferObject&)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_glImageRenderer;
}
