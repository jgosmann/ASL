#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->spinBox_Zoom, SIGNAL(valueChanged(int)), ui->glDisplay, SLOT(zoomImage(int)));
    connect(ui->glDisplay, SIGNAL(zoomChanged(int)), ui->spinBox_Zoom, SLOT(setValue(int)));
    connect(ui->pushButton_loadImage, SIGNAL(clicked()), ui->glDisplay, SLOT(loadImageFile()));
    connect(ui->pushButton_saveImage, SIGNAL(clicked()), ui->glDisplay, SLOT(saveImageFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_ApplyShaders_stateChanged(int _state)
{
    ui->glDisplay->enableShaders(_state);
}
