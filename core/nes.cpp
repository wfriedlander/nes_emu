#include "nes.h"

#include "bus.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "cartridge.h"
#include "controller.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <QDebug>


NES::NES(IVideo* video, IAudio* audio, IInput* input) : mVideo(video), mAudio(audio), mInput(input)
{
	mBus = std::make_unique<Bus>();
	mCPU = std::make_unique<CPU>(mBus.get());
	mPPU = std::make_unique<PPU>(mBus.get());
    mAPU = std::make_unique<APU>(mBus.get());
	mController = std::make_unique<Controller>();

	mPPU->SetVideoBackend(mVideo);
	mAPU->SetAudioBackend(mAudio);
	mController->SetInputBackend(mInput);

	mBus->Initialize(this, mCPU.get(), mPPU.get(), mAPU.get(), mController.get());
}

bool NES::LoadGame(std::string filename)
{
	mCartridge = Cartridge::Load(filename);
    if (!mCartridge) return false;
	mBus->LoadGame(mCartridge.get());
	mBus->Reset();
	mLast = std::chrono::high_resolution_clock::now();
    return true;
}

void NES::ResetGame()
{
    mBus->Reset();
}

json NES::Serialize()
{
    json state;
    state["bus"] = mBus->Serialize();
    state["cpu"] = mCPU->Serialize();
    state["ppu"] = mPPU->Serialize();
    state["apu"] = mAPU->Serialize();
    state["map"] = mCartridge->Serialize();
    return state;
}

void NES::Deserialize(json state)
{
    mBus->Deserialize(state["bus"]);
    mCPU->Deserialize(state["cpu"]);
    mPPU->Deserialize(state["ppu"]);
    mAPU->Deserialize(state["apu"]);
    mCartridge->Deserialize(state["map"]);
}

bool NES::Step()
{
    mDone = false;
    nes_time time = mCPU->CurrentCycle();
    nes_time next = mPPU->MinTimeToNMI();
    mCPU->RunUntil(time + next);
    nes_time new_time = mCPU->CurrentCycle();
    mPPU->RunUntil(new_time);
    mAPU->Execute(new_time - time);

//    nes_time cycle = mCPU->CurrentCycle();
//    mCPU->RunUntil(cycle + 1);
//    word cycles = mCPU->CurrentCycle() - cycle;
//    qDebug() << cycles;
//	mPPU->Execute(cycles * 3);
//    mAPU->Execute(cycles);
    return mDone;
}

void NES::FrameDone()
{
//	auto elapsed = std::chrono::high_resolution_clock::now() - mLast;
////	std::this_thread::sleep_for(std::chrono::milliseconds(16) - elapsed);
//////	auto e = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
////	std::this_thread::sleep_for(std::chrono::microseconds(mMicro) - elapsed);
//	auto post = std::chrono::high_resolution_clock::now() - mLast;
//	auto p = std::chrono::duration_cast<std::chrono::microseconds>(post).count();
//	mMicro += (16666 - p);
//	mLast = std::chrono::high_resolution_clock::now();
    mDone = true;
}

void NES::DebugKey(int key)
{
	//std::cout << key << "\n";
//	if (key == 22)
//		mCPU->EnableLogging(true);
//	else if (key == 16)
//		mCPU->EnableLogging(false);
}
