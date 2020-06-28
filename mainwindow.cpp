#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include <QEvent>
#include <QFile>
#include <QAudioDeviceInfo>
#include <QAudioOutput>


#include "mainwindow.h"
#include "interface.h"
#include "screen.h"

#include "input.h"


#include <iostream>
#include <queue>
//#include "serializable.h"


//class SubSerial : public Serializable
//{
//public:
//    SubSerial()
//    {
//        AddField("sub1", &sub1);
//        AddField("sub2", &sub2);
//    }

//    void reset()
//    {
//        sub1 = 0;
//        sub2 = 0;
//    }

//private:
//    byte sub1 = 13;
//    word sub2 = 2354;
//};

//class SerialTest : public Serializable
//{
//    struct sreg : reg//read only
//    {
//        byte v = 0;     // vertical blank started
//        byte s = 0;     // sprite 0 hit
//        byte o = 0;     // sprite overflow
//        void operator =(byte val) { v = (val >> 7) & 0x01; s = (val >> 6) & 0x01; o = (val >> 5) & 0x01; }
//        operator byte() const { return (v << 7 | s << 6 | o << 5); }
//    };

//public:
//    SerialTest()
//    {
//        AddField("reg", &mSreg);
//        AddField("byte", &mByte);
//        AddField("word", &mWord);
//        AddField("bool", &mBool);
//        AddField("array", (byte*)mBArr, 16);
//        AddField("marray", (byte*)mBMAr, 16*32);
//        AddField("serial", &mSub);

//        mSreg.v = 1;
//        mSreg.o = 1;
//    }

//    void reset()
//    {
//        mSreg = 0;
//        mByte = 0;
//        mWord = 0;
//        mBool = false;
//        for (int i = 0; i < 16; i++)
//            mBArr[i] = 0;
//        for (int i = 0; i < 16; i++)
//            for (int j = 0; j < 32; j++)
//                mBMAr[i][j] = 0;
//        mSub.reset();
//    }

//private:
//    sreg mSreg;
//    byte mByte = 23;
//    word mWord = 1034;
//    bool mBool = true;
//    byte mBArr[16] = { 0x50, 0x40, 0x56, 0x48, 0x64, 0x65, 0x43, 0x54, 0x87, 0x86, 0x47, 0x35, 0x43, 0x57, 0x86, 0x70 };
//    byte mBMAr[16][32] = { };
//    SubSerial mSub;
//};




class PulseBackend : public IAudio
{
    struct audiobuffer
    {
        float buffer[800] = {};
        int length = 0;
    };

public:
    PulseBackend()
    {
        QAudioFormat format;
        format.setSampleRate(48000);
        format.setChannelCount(1);
        format.setSampleType(QAudioFormat::Float);
        format.setSampleSize(sizeof(float) * 8);
        format.setCodec("audio/pcm");
        mOutput = new QAudioOutput(format);
        mDevice = mOutput->start();
        for (int i = 0; i < 20; i++)
        {
            mBuffers.push(audiobuffer());
        }
    }

public:
    void AudioUpdate(const AudioBuffer& buffer)
    {
        auto next = mBuffers.front();
        mBuffers.pop();

        for (auto sample : buffer)
        {
            mCount -= 1;
            if (mCount <= 0)
            {
                mCount += 37.2869375;
                next.buffer[next.length++] = sample;
            }
        }
        mQueued.push(next);

        while (mQueued.size() > 10)
        {
            auto abuf = mQueued.front();
            mDevice->write((char*)abuf.buffer, abuf.length * sizeof(float));
            mQueued.pop();
            abuf.length = 0;
            mBuffers.push(abuf);
        }

    }

private:
    QAudioOutput* mOutput;
    QIODevice* mDevice;
    double mCount = 0.0;
    std::queue<audiobuffer> mBuffers;
    std::queue<audiobuffer> mQueued;

};



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    SetupUI();

    auto video = new Screen(this);
    auto audio = new PulseBackend;
    auto input = new Input(this);

    mNes = new NES(video, audio, input);
    mLoader = new GameLoader(mNes);

    UpdateRecents();

    setCentralWidget(video);
    resize(512, 480 + menuBar()->height());

//    SerialTest st;
//    auto s = st.serialize();
//    auto a = s.dump(4);
//    std::cout << a << std::endl;

//    SerialTest st2;
//    st2.reset();
//    st2.deserialize(s);
//    mAudio = audio;

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
        mTimer = startTimer(16, Qt::TimerType::PreciseTimer);
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

