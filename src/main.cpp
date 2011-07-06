#include <QtGui/QApplication>
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gui::MainWindow w;
    w.show();

    QObject::connect(&w,SIGNAL(destroyed()),&a,SLOT(quit()));
    QObject::connect(&w,SIGNAL(exitProgram()),&a,SLOT(quit()));

    return a.exec();
}
