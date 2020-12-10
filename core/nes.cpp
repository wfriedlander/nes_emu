#include "nes.h"

#include "bus.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "mappers.h"
#include "controller.h"
#include "romloader.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <QDebug>


namespace Core {


NES::NES(Video::Interface* video, Audio::Interface* audio, Input::Interface* input) : mVideo(video), mAudio(audio), mInput(input)
{
    mBus = new Bus;
    mCPU = new CPU(mBus);
    mPPU = new PPU(mBus);
    mAPU = new APU(mBus);
    mController = new Controller;

    mLoader = new RomLoader;

	mPPU->SetVideoBackend(mVideo);
	mAPU->SetAudioBackend(mAudio);
	mController->SetInputBackend(mInput);

    mBus->Initialize(this, mCPU, mPPU, mAPU, mController);
}

NES::~NES()
{
    delete mBus;
    delete mCPU;
    delete mPPU;
    delete mAPU;
    delete mController;
    delete mLoader;
    if (mMapper) delete mMapper;
}

Result<std::string> NES::Load(std::string filename)
{
    auto result = mLoader->Load(filename);
    if (!result) {
        return {result.error, false};
    }

    if (mMapper) {
        delete mMapper;
    }

    mMapper = result;

    mBus->Load(mMapper);
	mBus->Reset();
    return filename + " loaded";
}

void NES::Reset()
{
    mBus->Reset();
}

void NES::Close()
{
    if (mMapper) {
        delete mMapper;
        mMapper = nullptr;
    }
}

bool NES::SaveState(int slot)
{
    if (mMapper == nullptr) return false;
    json state;
    state["bus"] = mBus->Serialize();
    state["cpu"] = mCPU->Serialize();
    state["ppu"] = mPPU->Serialize();
    state["apu"] = mAPU->Serialize();
    state["map"] = mMapper->Serialize();

    std::ofstream o("SaveStates/" + mMapper->RomName() + "_" + std::to_string(slot) + ".state");
    o << state;

    return true;
}

bool NES::LoadState(int slot)
{
    if (mMapper == nullptr) return false;

    std::ifstream i("SaveStates/" + mMapper->RomName() + "_" + std::to_string(slot) + ".state");
    if (!i.is_open()) return false;
    json state;
    i >> state;

    mBus->Deserialize(state["bus"]);
    mCPU->Deserialize(state["cpu"]);
    mPPU->Deserialize(state["ppu"]);
    mAPU->Deserialize(state["apu"]);
    mMapper->Deserialize(state["map"]);
    return true;
}

void NES::Step()
{
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
}

void NES::RunFrame()
{
    if (mMapper == nullptr) return;

    do {
        nes_time time = mCPU->CurrentCycle();
        nes_time next = mPPU->MinTimeToNMI();
        mCPU->RunUntil(time + next);
        nes_time new_time = mCPU->CurrentCycle();
        mPPU->RunUntil(new_time);
        mAPU->Execute(new_time - time);
    } while (mPPU->MinTimeToNMI() < 1000);
}

void NES::DebugKey(int key)
{
    (void)key;
	//std::cout << key << "\n";
//	if (key == 22)
//		mCPU->EnableLogging(true);
//	else if (key == 16)
//		mCPU->EnableLogging(false);
}



}
