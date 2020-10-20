#ifndef SETTINGS_H
#define SETTINGS_H


#include "interface_audio.h"
#include "interface_input.h"
#include "interface_video.h"



struct AudioSettings
{
    int device;
    int latency;
    int sample_rate;
    float master_volume;
    float channel_volume[5];
};


struct InputSettings
{
    ControllerInfo controller[2];
    ControllerInfo presets[10];
};


struct GeneralSettings
{
    int speed;
};


struct VideoSettings
{
    Filter filter;
    AspectRatio ratio;
    float scale;
    bool scale_lock;
    int scan_lines;
};




#endif // SETTINGS_H
