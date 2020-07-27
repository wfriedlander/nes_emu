#ifndef AUDIO_H
#define AUDIO_H

#include "interface.h"
#include "portaudio.h"
#include "blipbuffer.h"

#include <chrono>


class Audio : public IAudio
{
public:
    Audio();
    ~Audio();

public:
    void SetBuffer(Blip_Buffer* b);
    void Process(int16_t* out, int samples);

private:
    PaStream* stream;
    PaStreamParameters params;
    long target = 2400;
    Blip_Buffer* buffer = nullptr;
    decltype(std::chrono::high_resolution_clock::now()) start;
    double avail_rate = 0;
    long clock_rate = 1789773;
    double last_rate = 0;
    double delta_rate = 0;
};



#endif // AUDIO_H


