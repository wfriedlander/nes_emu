#pragma once

#include "common.h"
#include "nes.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "mappers.h"
#include "controller.h"
#include "serializer.h"

namespace Core {


class NES;


class Bus : public Serializable
{
public:
	Bus();

public:
    void Initialize(NES* nes, CPU* cpu, PPU* ppu, APU* apu, Controller* control);
    void Load(Mapper* mapper);
	void Reset();

public:
	void FrameComplete();
	void TriggerNMI();
	void TriggerIRQ();

public:
	byte CpuRead(word address);
	void CpuWrite(word address, byte value);
	byte PpuRead(word address);
	void PpuWrite(word address, byte value);

private:
	NES* mNES = nullptr;
	CPU* mCPU = nullptr;
	PPU* mPPU = nullptr;
    APU* mAPU = nullptr;
    Mapper* mMapper = nullptr;
	Controller* mController = nullptr;

private:
	std::vector<byte> mCpuRam;

};

}
