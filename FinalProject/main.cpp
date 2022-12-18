#include <QtWidgets/QApplication>
#include <qfontdatabase.h>
#include <string>

#include "mainwindow.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Create main application instance
    
    QFontDatabase::addApplicationFont(":/fonts/font_awesome_6_regular_free.otf"); // Add font awesome font to application
    QFontDatabase::addApplicationFont(":/fonts/font_awesome_6_solid_free.otf"); // Add font awesome font to application

    MainWindow w;
    w.setMouseTracking(true);
    w.resize(1250, 750);
    w.show();

    return a.exec();
}
