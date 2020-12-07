#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QFontDatabase>
#include <QFileInfo>
#include <QDir>

#include "core/romloader.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QDir::setCurrent(QFileInfo(argv[0]).absolutePath());
    QDir::current().mkpath("Saves");
    QDir::current().mkpath("SaveStates");

    QFontDatabase::addApplicationFont(":/fonts/Pretendo.ttf");

    RomLoader r;
    r.Load("donkey_kong.nes");
    r.Load("Super_mario_brothers.nes");
    r.Load("Super Mario Bros. (World).nes");
    r.Load("Tetris (U) [!].nes");
    r.Load("Tetris.nes");
    r.Load("Legend of Zelda, The (U) (PRG1) [!].nes");
    r.Load("Mike_tyson's_punch_out.nes");

    return 0;

    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.show();

    return a.exec();
}
