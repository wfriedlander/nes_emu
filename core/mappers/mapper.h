#pragma once

#include "common.h"


class Mapper
{
public:
    Mapper(Cartridge& cart);
    virtual ~Mapper() {};

public:
    byte CpuRead(word address);
    virtual void CpuWrite(word address, byte value) {};
    byte PpuRead(word address);
    void PpuWrite(word address, byte value);

public:
    virtual void EndScanline() {};

protected:
    bool MapPrg(int bank, int size, byte* mem);
    bool MapChr(int bank, int size, byte* mem);
    bool MapName(int bank, int size, byte* mem);

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
    std::vector<byte> palette;
    std::vector<byte> ram;
};


Mapper::Mapper(Cartridge& cart) : ram(2048), palette(32);
{
    mName = cart.name;
    mBattery = cart.battery;
    mChrRam = cart.vram_size > 0;
    mMirroring = cart.mirroring;

    trainer = cart.trainer;
    prg = cart.prg;
    chr = cart.chr;

    prg.resize(cart.prg_size * 1024);
    chr.resize((cart.chr_size + cart.vram_size) * 1024);
    wram.resize(cart.wram_size * 1024);
    sram.resize(cart.sram_size * 1024);
}


byte Mapper::CpuRead(word address)
{
    byte bank = address >> 13;
    return prg_map[bank-2][address & 0x1FFF];
}

byte Mapper::PpuRead(word address)
{

    switch (address >> 12)
    {
    case 0: 
    case 1: return chr_map[address >> 10][address & 0x3FF]
    case 2: return name_map[(address >> 10) & 3][address & 0x3FF];
    case 3: if (address & 0x100) return palette[address & 0x3 ? address : address & 0xF];
    default: return PpuRead(address - 0x1000);
    }
}

void Mapper::PpuWrite(word address, byte value)
{
    switch (address >> 12)
    {
    case 0: 
    case 1: if (mChrRam) chr_map[address >> 10][address & 0x3FF] = value; break;
    case 2: name_map[(address >> 10) & 3][address & 0x3FF] = value; break;
    case 3: if (address & 0x100) palette[address & 0x3 ? address : address & 0xF] = value; break;
    default: PpuWrite(address - 0x1000, value); break;
    }
};

// 6 banks of 8K slices from CPU 0x4000 to 0xFFFF
bool Mapper::MapPrg(int bank, int slices, byte* mem)
{
    if (bank + slices > 5) {
        return false;
    }
    for (int i = 0; i < slices) {
        prg_map[bank + i] = &mem[0x2000 * i];
    }
    return true;
}

// 8 banks of 1K slices from PPU 0x0000 to 0x1FFF
bool Mapper::MapChr(int bank, int slices, byte* mem)
{
    if (bank + slices > 7) {
        return false;
    }
    for (int i = 0; i < slices) {
        chr_map[bank + i] = &mem[0x400 * i];
    }
    return true;
}

// 4 banks of 1k slices from PPU 0x2000 to 0x2FFF
bool Mapper::MapName(int bank, int slices, byte* mem)
{
    if (bank + slices > 3) {
        return false;
    }
    for (int i = 0; i < slices) {
        name_map[bank + i] = &mem[0x400 * i];
    }
    return true;
}




class Mapper000 : public Mapper
{
public:
    Mapper000(Cartridge& cart);
    
};

Mapper000::Mapper000(Cartridge& cart) : Mapper(cart)
{
    MapPrg(2, 2, &prg[0]);
    if (cart.prg_size > 16) {
        MapPrg(4, 2, &prg[0x2000]);
    }
    else {
        MapPrg(4, 2, &prg[0]);
    }
    MapChr(0, 8, &chr[0]);
    if (mMirroring == 0) {
        MapName(0, 1, &ram[0]);
        MapName(1, 1, &ram[0]);
        MapName(2, 1, &ram[1024]);
        MapName(3, 1, &ram[1024]);
    } else {
        MapName(0, 2, &ram[0]);
        MapName(2, 2, &ram[0]);
    }
}


class Mapper001 : public Mapper
{
public:
    Mapper001(Cartridge& cart);
    ~Mapper001();

public:
    void CpuWrite(word address, byte value);

private:
    void RegistersChanged();

private:
    byte mReg[4];
    word mShift = 0x100;
};

Mapper001::Mapper001(Cartridge& cart) : Mapper(cart)
{
    // LOAD SAVE DATA HERE

    MapPrg(1, 2, &sram[0]);

    reg[0] = 0x0C;
    RegistersChanged();
}

Mapper001::~Mapper001()
{
    // STORE SAVE DATA HERE
}

void Mapper001::CpuWrite(word address, byte value)
{
    if (address & 0x8000)
    {
        mShift = (mShift << 1) | (value & 0x81);

        if (mShift & 0x80) {
            mShift = 0x200C | mReg[0];
            address = 0x8000;
        }

        if (mShift & 0x2000) {
            mReg[(address >> 13) & 0x03] = mShift;
            mShift = 0x100;
            RegistersChanged();
        }
    }
    else if (address & 0x6000)
    {
        if (!(mReg[3] & 0x80)) {
            ram[address & 0x1FFF] = value;
        }
    }
}

void Mapper001::RegistersChanged()
{
    switch (mReg[0] & 3)
    {
    case 0: MapName(0, 1, &ram[0]); MapName(1, 1, &ram[0]); MapName(1, 1, &ram[0]); MapName(1, 1, &ram[0]); break;
    case 1: MapName(0, 1, &ram[1024]); MapName(1, 1, &ram[1024]); MapName(1, 1, &ram[1024]); MapName(1, 1, &ram[1024]); break;
    case 2: MapName(0, 2, &ram[0]); MapName(0, 2, &ram[1024]); break;
    case 3: MapName(0, 1, &ram[0]); MapName(1, 1, &ram[0]); MapName(1, 1, &ram[1024]); MapName(1, 1, &ram[1024]); break;
    }

    switch ((mReg[0] >> 2) & 3)
    {
    case 0:
    case 1: MapPrg(2, 4, &prg[((mReg[3] >> 1) & 7) * 0x4000]); break;
    case 2: MapPrg(2, 2, &prg[0]); MapPrg(4, 2, &prg[(mReg[3] & 0xF) * 0x4000]]); break;
    case 3: MapPrg(2, 2, &prg[(mReg[3] & 0xF) * 0x4000]]); MapPrg(4, 2, &prg[prg.size() - 0x4000]); break;
    }

    switch (mReg[0] >> 4)
    {
    case 0: MapChr(0, 8, &chr[(mReg[1] >> 1) * 0x1000]); break;
    case 1: MapChr(0, 4, &chr[mReg[1] * 0x1000]); MapChr(4, 4, &chr[mReg[2] * 0x1000]); break;
    }
}

