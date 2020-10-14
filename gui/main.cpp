#include "mainwindow.h"
//#include "keyboard.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QFileInfo>
#include <QDir>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(QFileInfo(argv[0]).absolutePath());

    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;

//    Keyboard k(&a);

    w.show();
    return a.exec();
}
