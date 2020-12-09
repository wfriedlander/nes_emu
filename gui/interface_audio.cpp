#include "interface_audio.h"

#include <QDebug>


int paCallback(const void *, void *buffer, unsigned long frames, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *userData)
{
    Audio* backend = (Audio*)userData;
    int16_t* out = (int16_t*)buffer;
    backend->Process(out, frames);
    return paContinue;
}



Audio::Audio()
{
    Pa_Initialize();

    int dev = -1;
    double min_latency = 100;

    int numDevices = Pa_GetDeviceCount();
    for (int i = 0; i < numDevices; i++)
    {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        if (!deviceInfo->maxOutputChannels)
            continue;

//        qDebug() << deviceInfo->name << Pa_GetHostApiInfo( deviceInfo->hostApi )->name << deviceInfo->defaultLowOutputLatency;

        if (i != Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultOutputDevice)
            continue;

        if (deviceInfo->defaultLowOutputLatency < min_latency) {
            min_latency = deviceInfo->defaultLowOutputLatency;
            dev = i;
//            qDebug() << "dev" << i << deviceInfo->defaultLowOutputLatency;
        }
    }

    PaStreamParameters params;
    params.device = dev;
    params.channelCount = 1;
    params.sampleFormat = paInt16;
    params.suggestedLatency = 0.0;
    params.hostApiSpecificStreamInfo = NULL;

    auto fs = Pa_IsFormatSupported(NULL, &params, 48000.0);
    qDebug() << Pa_GetErrorText(fs);

    auto err = Pa_OpenStream(&mStream, NULL, &params, 48000, 200, paNoFlag, paCallback, this);
    qDebug() << Pa_GetErrorText(err);
}

Audio::~Audio()
{
    Pa_Terminate();
}

void Audio::SetSampleRate(long sample_rate)
{
    mSampleRate = sample_rate;
    mBuffer->set_sample_rate(sample_rate);
    // GONNA TAKE A LOT MORE THAN THIS
}

void Audio::SetLatency(long milliseconds)
{
    mLatency = milliseconds;
    mBuffer->clear();
    mFillTarget = (mSampleRate / 1000.0) * milliseconds;
    mFillLevel = mFillTarget;
    mLastFill = mFillTarget;
    auto clocks = mBuffer->count_clocks(mFillTarget);
    mBuffer->end_frame(clocks);

    auto available = mBuffer->samples_avail();
    qDebug() << "set latency" << milliseconds << mFillTarget << available;
}

void Audio::Pause()
{
    if (Pa_IsStreamActive(mStream) > 0) {
        Pa_StopStream(mStream);
    }
}

void Audio::Play()
{
    qDebug() << "play" << mBuffer->samples_avail();
    if (Pa_IsStreamStopped(mStream) > 0) {
        Pa_StartStream(mStream);
        mStart = std::chrono::high_resolution_clock::now();
    }
}

void Audio::Process(int16_t *out, int samples)
{
    auto available = mBuffer->samples_avail();
    mBuffer->read_samples((blip_sample_t*)out, samples);

    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - mStart;

    if (elapsed > std::chrono::milliseconds(25)) {
        mLastFill = mFillLevel;
        mFillLevel = (mFillLevel * 0.95) + (available * 0.05);
        mDeltaFill = (mDeltaFill * 0.95) + (mFillLevel - mLastFill) * 0.05;

        auto delta = mFillTarget - mFillLevel;
        auto target_rate = delta / 300;

        mClockRate += (mDeltaFill - target_rate) * 50;

//        qDebug() << mFillLevel << mDeltaFill << target_rate << mClockRate;

        mBuffer->clock_rate(mClockRate);
        mStart = now;
    }
}

void Audio::SetBuffer(Blip_Buffer *b)
{
    mBuffer = b;
    mBuffer->clock_rate(mClockRate);
    SetSampleRate(48000);
    SetLatency(60);
}

