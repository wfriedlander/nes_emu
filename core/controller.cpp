#include "controller.h"

#include <cstdio>


Controller::Controller()
{
}

void Controller::SetInputBackend(IInput* input)
{
	mInput = input;
}

byte Controller::RegisterRead(word address)
{
	if (address == 0)
	{
        byte r = mStateLatch0 >> 7;
        mStateLatch0 = mStateLatch0 << 1;
		return r;
	}
	else
	{
        byte r = mStateLatch1 >> 7;
        mStateLatch1 = mStateLatch1 << 1;
		return r;
	}
	
}

void Controller::RegisterWrite(byte value)
{
    if (value & 1) {
        Latch();
	}
}

void Controller::Latch()
{
    auto state0 = mInput->GetState(0);

    mStateLatch0 = 0;
    mStateLatch0 |= state0[Button::A] ? 0x80 : 0;
    mStateLatch0 |= state0[Button::B] ? 0x40 : 0;
    mStateLatch0 |= state0[Button::Select] ? 0x20 : 0;
    mStateLatch0 |= state0[Button::Start] ? 0x10 : 0;
    mStateLatch0 |= state0[Button::Up] ? 0x08 : 0;
    mStateLatch0 |= state0[Button::Down] ? 0x04 : 0;
    mStateLatch0 |= state0[Button::Left] ? 0x02 : 0;
    mStateLatch0 |= state0[Button::Right] ? 0x01 : 0;

    auto state1 = mInput->GetState(1);

    mStateLatch1 = 0;
    mStateLatch1 |= state1[Button::A] ? 0x80 : 0;
    mStateLatch1 |= state1[Button::B] ? 0x40 : 0;
    mStateLatch1 |= state1[Button::Select] ? 0x20 : 0;
    mStateLatch1 |= state1[Button::Start] ? 0x10 : 0;
    mStateLatch1 |= state1[Button::Up] ? 0x08 : 0;
    mStateLatch1 |= state1[Button::Down] ? 0x04 : 0;
    mStateLatch1 |= state1[Button::Left] ? 0x02 : 0;
    mStateLatch1 |= state1[Button::Right] ? 0x01 : 0;
}

