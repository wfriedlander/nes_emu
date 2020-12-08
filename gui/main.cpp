#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QFontDatabase>
#include <QFileInfo>
#include <QDir>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QDir::setCurrent(QFileInfo(argv[0]).absolutePath());
    QDir::current().mkpath("Saves");
    QDir::current().mkpath("SaveStates");

    QFontDatabase::addApplicationFont(":/fonts/Pretendo.ttf");

    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.show();

    return a.exec();
}
