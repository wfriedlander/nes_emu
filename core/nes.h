#pragma once

#include "common.h"
#include "interface.h"
#include "json.h"


class NES
{
public:
    NES(IVideo* video, IAudio* audio, IInput* input);
    ~NES();

public:
    Result<std::string> Load(std::string filename);
    void Reset();
    void Close();

    bool SaveState(int slot);
    bool LoadState(int slot);

    void RunFrame();
    void Step();

public:
	void DebugKey(int key);

private:
    class Bus* mBus = nullptr;
    class CPU* mCPU = nullptr;
    class PPU* mPPU = nullptr;
    class APU* mAPU = nullptr;
    class Mapper* mMapper = nullptr;
    class Controller* mController = nullptr;

    class RomLoader* mLoader = nullptr;

    IVideo* mVideo = nullptr;
    IAudio* mAudio = nullptr;
    IInput* mInput = nullptr;
};

