#pragma once

#include <cstdint>
#include <vector>


using ScreenBuffer = uint8_t[240][256];

class IVideo
{
public:
    virtual void VideoUpdate(const ScreenBuffer& buffer) = 0;
};


using AudioBuffer = std::vector<float>;
class Blip_Buffer;

class IAudio
{
public:
//    virtual void AudioUpdate(const AudioBuffer& buffer) = 0;
    virtual void SetBuffer(Blip_Buffer* buffer) = 0;
};

class Blip_Buffer;

class BAudio
{
public:
    virtual void SetBuffer(Blip_Buffer* buffer) = 0;
};



enum class Button : int
{
    A = 0,
    B,
    Start,
    Select,
    Up,
    Down,
    Left,
    Right
};

struct ControllerState
{
    bool& operator[](Button b) {return button_state[static_cast<int>(b)];}
private:
    bool button_state[8];
};


class IInput
{
public:
    virtual ControllerState GetController(int controller) = 0;
};

