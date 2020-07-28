#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QEvent>
#include <QFile>


#include "mainwindow.h"
#include "video.h"
#include "input.h"
#include "audio.h"
#include "controllerinput.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    SetupUI();

    auto video = new Video(this);
    auto audio = new Audio;
    mInput = new Input(this);

    mNes = new NES(video, audio, mInput);
    mLoader = new GameLoader(mNes);

    UpdateRecents();

    setCentralWidget(video);
    resize(512, 480 + menuBar()->height());
}

MainWindow::~MainWindow()
{

}

void MainWindow::Open()
{
    auto filename = QFileDialog::getOpenFileName(this, "Open ROM");
    qDebug() << "filename " << filename;
    LoadGame(filename);
}

void MainWindow::Play()
{
    if (!mPlaying)
    {
        mPlaying = true;
        mLast = std::chrono::high_resolution_clock::now();
        mTimer = startTimer(17, Qt::TimerType::PreciseTimer);
    }
}

void MainWindow::Pause()
{
    if (mPlaying)
    {
        mPlaying = false;
        killTimer(mTimer);
        mTimer = -1;
    }
}

void MainWindow::Reset()
{
    mNes->ResetGame();
}

void MainWindow::SaveState(int state)
{
    qDebug() << "Save " << state;

    json save_state = mNes->Serialize();

    std::ofstream o("state" + std::to_string(state) + ".json");
    o << save_state;
}

void MainWindow::LoadState(int state)
{
    qDebug() << "Load " << state;

    std::ifstream i("state" + std::to_string(state) + ".json");
    if (!i.is_open()) return;
    json load_state;
    i >> load_state;
    mNes->Deserialize(load_state);
}

void MainWindow::GeneralSettings()
{

}

void MainWindow::AudioSettings()
{

}

void MainWindow::VideoSettings()
{

}

void MainWindow::ControllerSettings()
{
	ControllerInput* settings = new ControllerInput();
	settings->getInput(mInput);
	settings->setWindowTitle("Change Controller Settings");
	settings->show();
}

void MainWindow::About()
{

}

void MainWindow::LoadGame(const QString &filename)
{
    if (!mLoader->LoadGame(filename)) {
        qDebug() << "Not a game";
        return;
    }

    UpdateRecents();
    Play();
}

void MainWindow::UpdateRecents()
{
    mRecent->clear();
    for (auto& game : mLoader->RecentGames())
    {
        auto file = QFileInfo(game);
        auto action = new QAction(file.fileName(), mRecent);
        action->setToolTip(game);
        mRecent->addAction(action);
    }
    mRecent->setEnabled(!mRecent->isEmpty());
}

void MainWindow::timerEvent(QTimerEvent *)
{
    while(!mNes->Step());

//    auto elapsed = std::chrono::high_resolution_clock::now() - mLast;
//    auto p = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
//    qDebug() << mMicro << p;
//    mMicro += (16666 - p);
//    mLast = std::chrono::high_resolution_clock::now();
//    killTimer(mTimer);
//    mTimer = startTimer(mMicro / 1000, Qt::TimerType::PreciseTimer);

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() != QEvent::MouseMove)
        qDebug() << obj << event->type();
    return false;
}

void MainWindow::SetupUI()
{
    // MENU BAR
    auto menubar = new QMenuBar(this);
    setMenuBar(menubar);

    // FILE MENU
    auto actionOpen = new QAction("Open", this);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::Open);
    actionOpen->setShortcut(QString("CTRL+O"));
    auto actionExit = new QAction("Exit", this);
    connect(actionExit, &QAction::triggered, QApplication::instance(), &QApplication::quit);
    auto menuFile = new QMenu("File", menubar);
    mRecent = new QMenu("Recent", menuFile);
    mRecent->setToolTipsVisible(true);
    connect(mRecent, &QMenu::triggered, [&](QAction* a) { LoadGame(a->toolTip()); });
    menuFile->addAction(actionOpen);
    menuFile->addSeparator();
    menuFile->addMenu(mRecent);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);
    menubar->addMenu(menuFile);

    // EMULATION MENU
    auto actionPlay = new QAction("Play", this);
    connect(actionPlay, &QAction::triggered, this, &MainWindow::Play);
    auto actionPause = new QAction("Pause", this);
    connect(actionPause, &QAction::triggered, this, &MainWindow::Pause);
    auto actionReset = new QAction("Reset", this);
    connect(actionReset, &QAction::triggered, this, &MainWindow::Reset);
    auto menuEmulation = new QMenu("Emulation", menubar);
    menuEmulation->addAction(actionPlay);
    menuEmulation->addAction(actionPause);
    menuEmulation->addAction(actionReset);
    menuEmulation->addSeparator();
    auto menuSaveState = new QMenu("Save State", menuEmulation);
    for (int i = 1; i <= 10; i++) {
        auto state = new QAction("State " + QString::number(i), this);
        state->setShortcut("CTRL+F" + QString::number(i));
        connect(state, &QAction::triggered, [&, i](){SaveState(i);});
        menuSaveState->addAction(state);
    }
    menuEmulation->addMenu(menuSaveState);
    auto menuLoadState = new QMenu("Load State", menuEmulation);
    for (int i = 1; i <= 10; i++) {
        auto state = new QAction("State " + QString::number(i), this);
        state->setShortcut("CTRL+SHIFT+F" + QString::number(i));
        connect(state, &QAction::triggered, [&, i](){LoadState(i);});
        menuLoadState->addAction(state);
    }
    menuEmulation->addMenu(menuLoadState);
    menubar->addMenu(menuEmulation);

    // OPTIONS MENU
    auto actionSettings = new QAction("General Settings", this);
    connect(actionSettings, &QAction::triggered, this, &MainWindow::GeneralSettings);
    auto actionAudio = new QAction("Audio Settings", this);
    connect(actionAudio, &QAction::triggered, this, &MainWindow::AudioSettings);
    auto actionVideo = new QAction("Video Settings", this);
    connect(actionVideo, &QAction::triggered, this, &MainWindow::VideoSettings);
    auto actionController = new QAction("Controller Settings", this);
    connect(actionController, &QAction::triggered, this, &MainWindow::ControllerSettings);
    auto menuOptions = new QMenu("Options", menubar);
    menuOptions->addAction(actionSettings);
    menuOptions->addAction(actionAudio);
    menuOptions->addAction(actionVideo);
    menuOptions->addAction(actionController);
    menubar->addMenu(menuOptions);

    // HELP MENU
    auto actionAbout = new QAction("About", this);
    connect(actionAbout, &QAction::triggered, this, &MainWindow::About);
    auto menuHelp = new QMenu("Help", menubar);
    menuHelp->addAction(actionAbout);
    menubar->addMenu(menuHelp);

//    menubar->installEventFilter(this);

//    auto statusbar = new QStatusBar(this);
//    setStatusBar(statusbar);

//    connect(menubar, &QMenuBar::triggered, this, []() { qDebug() << "menubar triggered"; });
}

