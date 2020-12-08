#pragma once

#include "core/common.h"
#include "core/serializer.h"


struct Cartridge
{
    std::string name;
    std::vector<byte> trainer;
    std::vector<byte> prg;
    std::vector<byte> chr;

    int mirroring = 0;
    int prg_size = 0;
    int chr_size = 0;
    int vram_size = 0;
    int wram_size = 0;
    int sram_size = 0;
    bool battery = false;
};



class Mapper : public Serializable
{
public:
    Mapper(Cartridge& cart);
    virtual ~Mapper() {};

public:
    byte CpuRead(word address);
    virtual void CpuWrite(word address, byte value) {(void)address; (void)value;};
    byte PpuRead(word address);
    void PpuWrite(word address, byte value);

public:
    virtual void EndScanline() {};

protected:
    bool MapPrg(int bank, int size, byte* mem);
    bool MapChr(int bank, int size, byte* mem);
    bool MapName(int bank, int size, byte* mem);

    byte* PrgBank(int bank);
    byte* ChrBank(int bank);
    byte* PrgHalfBank(int bank);
    byte* ChrHalfBank(int bank);

protected:
    std::string mName;
    bool mBattery = false;
    bool mChrRam = false;
    int mMirroring = 0;
    
    byte* prg_map[6];
    byte* chr_map[8];
    byte* name_map[4];

    std::vector<byte> trainer;
    std::vector<byte> prg;
    std::vector<byte> chr;
    std::vector<byte> wram;
    std::vector<byte> sram;
    std::vector<byte> ram;
    std::vector<byte> palette;
    std::vector<byte> unmapped;
};



