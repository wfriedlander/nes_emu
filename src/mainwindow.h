#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "nes.h"

class GameLoader;
class Input;
class Audio;
class Video;


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
    void RunFrame();
    void UpdateRecents();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;;

private:
    void SetupUI();

private:
    GameLoader* mLoader = nullptr;
    Input* mInput = nullptr;
    Audio* mAudio = nullptr;
    Video* mVideo = nullptr;
    NES* mNes = nullptr;
    QMenu* mRecent;

private:
    bool mPlaying = false;
    std::chrono::high_resolution_clock::time_point mLastFrameTime;
    double mFrameTimeAverage = 16.0;

};
#endif // MAINWINDOW_H
