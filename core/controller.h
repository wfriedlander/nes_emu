#pragma once

#include "common.h"
#include "interface.h"

namespace Core {


class Controller
{
public:
	Controller();
    void SetInputBackend(Input::Interface* input);

public:
	byte RegisterRead(word address);
	void RegisterWrite(byte value);

private:
    void Latch();

private:
    Input::Interface* mInput = nullptr;
    byte mStateLatch0 = 0;
    byte mStateLatch1 = 0;
};

}
