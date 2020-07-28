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

class IAudio
{
public:
    virtual void AudioUpdate(const AudioBuffer& buffer) = 0;
};


struct ButtonState
{
	bool a;
	bool b;
	bool select;
	bool start;
	bool up;
	bool down;
	bool left;
	bool right;
};

class IInput
{
public:
    virtual const ButtonState& InputUpdate() = 0;
};

