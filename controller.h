#pragma once

#include "common.h"
#include "interface.h"


class Controller
{
public:
	Controller();
    void SetInputBackend(IInput* input);

public:
	byte RegisterRead(word address);
	void RegisterWrite(byte value);

private:
	void Latch();

private:
    IInput* mInput = nullptr;
	byte state0 = 0;
	byte state1 = 0;
};

