#ifndef STORAGE_H
#define STORAGE_H

#include "settings.h"

#include <QObject>


class Storage : public QObject
{
    Q_OBJECT
public:
    explicit Storage(QObject *parent = nullptr);

public:
    void SaveState();
    void LoadState();

    void SaveRAM(byte* RAM, int length);
    void LoadRAM(byte* RAM, int length);

    AudioSettings GetAudioSettings();
    void SaveAudioSettings(AudioSettings s);

    GeneralSettings GetGeneralSettings();
    void SaveGeneralSettings(GeneralSettings s);

    InputSettings GetInputSettings();
    void SaveInputSettings(InputSettings s);

    VideoSettings GetVideoSettings();
    void SaveVideoSettings(VideoSettings s);

signals:

};

#endif // CONFIGURATIONMANAGER_H



// AUDIO
/// latency
/// device
/// volume
/// sample rate
/// channel volume

// VIDEO
/// scale
/// aspect ratio
/// filter
/// scan lines
/// palette

// INPUT
/// current mapping
/// presets

// GENERAL
/// shortcuts
/// speed
/// recent game list

// OTHER
/// save states
/// save files
/// last state per game?
