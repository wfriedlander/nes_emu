#pragma once

#include "common.h"
#include "serializer.h"


struct CartridgeConfig
{
	byte prg_size;
	byte chr_size;
	byte mirroring;
	byte persistent;
	byte trainer;
	byte four_screen;
};


class Cartridge : public Serializable
{
public:
	static std::unique_ptr<Cartridge> Load(std::string filename);

public:
	virtual byte CpuRead(word address) = 0;
	virtual void CpuWrite(word address, byte value) = 0;
	virtual byte PpuRead(word address) = 0;
	virtual void PpuWrite(word address, byte value) = 0;
};




template <int N>
struct CartSegment
{
    operator char* ()
    {
        return reinterpret_cast<char*>(&mem[0]);
    }
    byte mem[N];
};



class NewMapper
{
protected:
    std::vector<CartSegment<16384>> mPrgBank;
    std::vector<CartSegment<8192>> mChrBank;
    std::vector<byte> mPpuRam;
    std::vector<byte> mPaletteRam;
    byte mMirroring = 0;
    byte mPersistent = 0;
    byte mFourScreen = 0;
};


class NewCartridge
{
public:
    byte CpuRead(word address);
    void CpuWrite(word address, byte value);
    byte PpuRead(word address);
    void PpuWrite(word address, byte value);

private:
    NewMapper* mMapper = nullptr;


};
