#pragma once

#include "common.h"
#include "interface.h"
#include "json.h"

namespace Core {


class NES
{
public:
    NES(Core::Video::Interface* video, Core::Audio::Interface* audio, Core::Input::Interface* input);
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

    Video::Interface* mVideo = nullptr;
    Audio::Interface* mAudio = nullptr;
    Input::Interface* mInput = nullptr;
};


}
