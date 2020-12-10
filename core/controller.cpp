#include "controller.h"

#include <cstdio>


namespace Core {


Controller::Controller()
{
}

void Controller::SetInputBackend(Input::Interface* input)
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
    auto state0 = mInput->State(0);

    mStateLatch0 = 0;
    mStateLatch0 |= state0[Input::A] ? 0x80 : 0;
    mStateLatch0 |= state0[Input::B] ? 0x40 : 0;
    mStateLatch0 |= state0[Input::Select] ? 0x20 : 0;
    mStateLatch0 |= state0[Input::Start] ? 0x10 : 0;
    mStateLatch0 |= state0[Input::Up] ? 0x08 : 0;
    mStateLatch0 |= state0[Input::Down] ? 0x04 : 0;
    mStateLatch0 |= state0[Input::Left] ? 0x02 : 0;
    mStateLatch0 |= state0[Input::Right] ? 0x01 : 0;

    auto state1 = mInput->State(1);

    mStateLatch1 = 0;
    mStateLatch1 |= state1[Input::A] ? 0x80 : 0;
    mStateLatch1 |= state1[Input::B] ? 0x40 : 0;
    mStateLatch1 |= state1[Input::Select] ? 0x20 : 0;
    mStateLatch1 |= state1[Input::Start] ? 0x10 : 0;
    mStateLatch1 |= state1[Input::Up] ? 0x08 : 0;
    mStateLatch1 |= state1[Input::Down] ? 0x04 : 0;
    mStateLatch1 |= state1[Input::Left] ? 0x02 : 0;
    mStateLatch1 |= state1[Input::Right] ? 0x01 : 0;
}


}
