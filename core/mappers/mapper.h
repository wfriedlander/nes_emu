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
    enum MIRRORING {
        SINGLE_LOW,
        SINGLE_HIGH,
        VERTICAL,
        HORIZONTAL,
    };

    enum REGION {
        CPU_LOW,    // CPU 0x4000 8k
        SAVE_RAM,   // CPU 0x6000 8k
        PRG_0,      // CPU 0x8000 8k (16k)
        PRG_0B,     // CPU 0xA000 8k
        PRG_1,      // CPU 0xC000 8k (16k)
        PRG_1B,     // CPU 0xE000 8k
        CHR_0,      // PPU 0x0000 1k (4k)
        CHR_0B,     // PPU 0x0400 1k
        CHR_0C,     // PPU 0x0800 1k
        CHR_0D,     // PPU 0x0C00 1k
        CHR_1,      // PPU 0x1000 1k (4k)
        CHR_1B,     // PPU 0x1400 1k
        CHR_1C,     // PPU 0x1800 1k
        CHR_1D,     // PPU 0x1C00 1k
        NAME_0,     // PPU 0x2000 1k
        NAME_1,     // PPU 0x2400 1k
        NAME_2,     // PPU 0x2800 1k
        NAME_3,     // PPU 0x2C00 1k
    };


protected:
//    bool MapPrg(int bank, int size, byte* mem);
//    bool MapChr(int bank, int size, byte* mem);
//    bool MapName(int bank, int size, byte* mem);

//    byte* PrgBank(int bank);
//    byte* ChrBank(int bank);
//    byte* PrgHalfBank(int bank);
//    byte* ChrHalfBank(int bank);

    void MapNametable(MIRRORING mirroring);
    void MapRegion(REGION region, int kb_size, std::vector<byte>& mem, int bank=0, int kb_bank_size=-1);

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



