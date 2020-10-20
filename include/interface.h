#pragma once

#include <cstdint>


using ScreenBuffer = uint8_t[240][256];

class IVideo
{
public:
    virtual void VideoUpdate(const ScreenBuffer& buffer) = 0;
};


class Blip_Buffer;

class IAudio
{
public:
    virtual void SetBuffer(Blip_Buffer* buffer) = 0;
};


enum class Button : int
{
    Up = 0,
    Down,
    Left,
    Right,
    Start,
    Select,
    A,
    B,
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

