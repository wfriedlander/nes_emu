#include "audio.h"

#include <QDebug>


int paCallback(const void *, void *buffer, unsigned long frames, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *userData)
{
    Audio* backend = (Audio*)userData;
    int16_t* out = (int16_t*)buffer;
    backend->Process(out, frames);
    return 0;
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

        if (i != Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultOutputDevice)
            continue;

        if (deviceInfo->defaultLowOutputLatency < min_latency) {
            min_latency = deviceInfo->defaultLowOutputLatency;
            dev = i;
            qDebug() << "dev" << i << deviceInfo->defaultLowOutputLatency;
        }
    }

    params.device = dev;
    params.channelCount = 1;
    params.sampleFormat = paInt16;
    params.suggestedLatency = 0.0;
    params.hostApiSpecificStreamInfo = NULL;

    auto fs = Pa_IsFormatSupported(NULL, &params, 48000.0);
    qDebug() << Pa_GetErrorText(fs);

    auto err = Pa_OpenStream(&stream, NULL, &params, 48000, 200, paNoFlag, paCallback, this);
    qDebug() << Pa_GetErrorText(err);
}

Audio::~Audio()
{
    Pa_Terminate();
}

void Audio::Process(int16_t *out, int samples)
{
    auto available = buffer->samples_avail();
    long removed = buffer->read_samples((blip_sample_t*)out, samples);

    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - start;

    if (elapsed > std::chrono::milliseconds(25)) {
        last_rate = avail_rate;
        avail_rate = (avail_rate * 0.95) + (available * 0.05);
        delta_rate = (delta_rate * 0.95) + (avail_rate - last_rate) * 0.05;

        auto delta = target - avail_rate;
        auto target_rate = delta / 500;

        if (delta_rate > target_rate) {
            clock_rate += 125;
        }
        else {
            clock_rate -= 125;
        }
        //            qDebug() << avail_rate << delta << delta_rate << target_rate;

        buffer->clock_rate(clock_rate);
        start = now;
    }
}

void Audio::SetBuffer(Blip_Buffer *b)
{
    buffer = b;
    Pa_StartStream(stream);
    start = std::chrono::high_resolution_clock::now();
}

