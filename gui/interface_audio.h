#ifndef AUDIO_H
#define AUDIO_H

#include "core/interface.h"
#include "core/blipbuffer.h"
#include "portaudio/portaudio.h"

#include <chrono>


class Audio : public IAudio
{
public:
    Audio();
    ~Audio();

public:
    void SetSampleRate(long sample_rate);
    void SetLatency(long milliseconds);
    void Pause();
    void Play();

public:
    void SetBuffer(Blip_Buffer* b);
    void Process(int16_t* out, int samples);


private:
    PaStream* mStream;
    Blip_Buffer* mBuffer = nullptr;

    long mSampleRate = 48000;
    long mLatency = 60;

    std::chrono::high_resolution_clock::time_point mStart;
    long mClockRate = 1789773;
    long mFillTarget = 2880;
    double mFillLevel = 2880;
    double mLastFill = 2880;
    double mDeltaFill = 0;

};



#endif // AUDIO_H


