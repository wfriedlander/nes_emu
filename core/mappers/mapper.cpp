
#include "mapper.h"


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

