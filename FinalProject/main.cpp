#include <QtWidgets/QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMouseTracking(true);
    w.resize(900, 600);
    w.show();
    return a.exec();
}
