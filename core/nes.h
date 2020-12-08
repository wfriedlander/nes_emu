#pragma once

#include "common.h"
#include "interface.h"
#include "json.h"


class NES
{
public:
    NES(IVideo* video, IAudio* audio, IInput* input);

public:
    Result<std::string> Load(std::string filename);
    void Reset();

    json Serialize();
    void Deserialize(json state);

    void RunFrame();
    void Step();

public:
	void DebugKey(int key);

private:
    std::unique_ptr<class Bus> mBus;
    std::unique_ptr<class CPU> mCPU;
    std::unique_ptr<class PPU> mPPU;
    std::unique_ptr<class APU> mAPU;
    std::unique_ptr<class Mapper> mMapper;
    std::unique_ptr<class Controller> mController;

    std::unique_ptr<class RomLoader> mLoader;

    IVideo* mVideo = nullptr;
    IAudio* mAudio = nullptr;
    IInput* mInput = nullptr;
};

