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

    connect(ui->spinBox_Zoom, SIGNAL(valueChanged(int)), ui->glDisplay, SLOT(setImageZoom(int)));
    connect(ui->glDisplay, SIGNAL(zoomChanged(int)), ui->spinBox_Zoom, SLOT(setValue(int)));
    connect(ui->action_LoadImage, SIGNAL(changed()), ui->glDisplay, SLOT(loadImageFile()));
    connect(ui->action_SaveImage, SIGNAL(changed()), ui->glDisplay, SLOT(saveImageFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_ApplyShaders_stateChanged(int _state)
{
    ui->glDisplay->enableShaders(_state);
}
