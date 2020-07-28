#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "nes.h"
#include "gameloader.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void Open();
    void Play();
    void Pause();
    void Reset();
    void SaveState(int state);
    void LoadState(int state);
    void GeneralSettings();
    void AudioSettings();
    void VideoSettings();
    void ControllerSettings();
    void About();

private:
    void LoadGame(const QString& filename);
    void UpdateRecents();

protected:
    void timerEvent(QTimerEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;;

private:
    void SetupUI();

private:
    GameLoader* mLoader = nullptr;
    NES* mNes = nullptr;
    bool mPlaying = false;
    int mTimer = -1;
	Input* mInput = new Input(this);
private:
    QMenu* mRecent;
    std::chrono::time_point<std::chrono::high_resolution_clock> mLast;
    int mMicro = 16666;

};
#endif // MAINWINDOW_H
