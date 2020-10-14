#pragma once

#include "common.h"
#include "core/bus.h"
#include "core/cpu.h"
#include "core/ppu.h"
#include "core/apu.h"
#include "core/cartridge.h"
#include "core/controller.h"
#include "interface.h"
#include "json.h"


class Bus;
class CPU;
class PPU;
class APU;
class Cartridge;
class Controller;


class NES
{
public:
    NES(IVideo* video, IAudio* audio, IInput* input);

public:
	bool LoadGame(std::string filename);
	void ResetGame();
    json Serialize();
    void Deserialize(json state);

    bool Step();
	void FrameDone();

public:
	void DebugKey(int key);

private:
	std::unique_ptr<Bus> mBus;
	std::unique_ptr<CPU> mCPU;
	std::unique_ptr<PPU> mPPU;
    std::unique_ptr<APU> mAPU;
	std::unique_ptr<Cartridge> mCartridge;
	std::unique_ptr<Controller> mController;

    IVideo* mVideo = nullptr;
    IAudio* mAudio = nullptr;
    IInput* mInput = nullptr;

	std::chrono::time_point<std::chrono::high_resolution_clock> mLast;
    int64_t mMicro = 16666;
    bool mDone = false;

};

