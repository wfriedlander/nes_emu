
#include "mapper.h"

#include <iostream>


Mapper::Mapper(Cartridge& cart) : ram(2048), palette(32), unmapped(8192)
{
    mName = cart.name;
    mBattery = cart.battery;
    mChrRam = cart.vram_size > 0;
    mMirroring = cart.mirroring;

    trainer = cart.trainer;
    prg = cart.prg;
    chr = cart.chr;

//    prg.resize(cart.prg_size * 1024);
    chr.resize((cart.chr_size + cart.vram_size) * 1024);
    wram.resize(cart.wram_size * 1024);
    sram.resize(cart.sram_size * 1024);

    for (int i = 0; i < 6; i++) MapPrg(i, 1, &unmapped[0]);
    for (int i = 0; i < 8; i++) MapChr(i, 1, &unmapped[0]);
    for (int i = 0; i < 4; i++) MapName(i, 1, &unmapped[0]);
}


byte Mapper::CpuRead(word address)
{
    byte bank = address >> 13;
//    std::cout << address << " " << bank << "\n";
    return prg_map[bank-2][address & 0x1FFF];
}

byte Mapper::PpuRead(word address)
{

    switch (address >> 12)
    {
    case 0:
    case 1: return chr_map[address >> 10][address & 0x3FF];
    case 2: return name_map[(address >> 10) & 3][address & 0x3FF];
    case 3: if (address & 0x100) return palette[address & 0x3 ? address & 0x1F : address & 0x0F];
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
    case 3: if (address & 0x100) palette[address & 0x3 ? address & 0x1F : address & 0x0F] = value; break;
    default: PpuWrite(address - 0x1000, value); break;
    }
};

// 6 banks of 8K slices from CPU 0x4000 to 0xFFFF
bool Mapper::MapPrg(int bank, int slices, byte* mem)
{
    std::cout << "prg map " << bank << " " << slices << "\n";
    if (bank + slices > 6) {
        return false;
    }
    for (int i = 0; i < slices; i++) {
        prg_map[bank + i] = &mem[0x2000 * i];
    }
    return true;
}

// 8 banks of 1K slices from PPU 0x0000 to 0x1FFF
bool Mapper::MapChr(int bank, int slices, byte* mem)
{
    std::cout << "chr map " << bank << " " << slices << "\n";
    if (bank + slices > 8) {
        return false;
    }
    for (int i = 0; i < slices; i++) {
        chr_map[bank + i] = &mem[0x400 * i];
    }
    return true;
}

// 4 banks of 1k slices from PPU 0x2000 to 0x2FFF
bool Mapper::MapName(int bank, int slices, byte* mem)
{
    std::cout << "name map " << bank << " " << slices << "\n";
    if (bank + slices > 4) {
        return false;
    }
    for (int i = 0; i < slices; i++) {
        name_map[bank + i] = &mem[0x400 * i];
    }
    return true;
}

byte* Mapper::PrgBank(int bank)
{
    int banks = (prg.size() >> 14);
    std::cout << "prg bank " << bank << " adjusted " << ((banks + bank % banks) & (banks - 1)) << "\n";
    return &prg[((banks + bank % banks) & (banks - 1)) * 0x4000];
}

byte* Mapper::ChrBank(int bank)
{
    int banks = (chr.size() >> 13);
    std::cout << "chr bank " << bank << " adjusted " << ((banks + bank % banks) & (banks - 1)) << "\n";
    return &chr[((banks + bank % banks) & (banks - 1)) * 0x2000];
}

byte* Mapper::PrgHalfBank(int bank)
{
    std::cout << "prg half bank " << bank << "\n";
    return &PrgBank(bank >> 1)[(bank & 1) * 0x2000];
}

byte* Mapper::ChrHalfBank(int bank)
{
    std::cout << "chr half bank " << bank << "\n";
    return &ChrBank(bank >> 1)[(bank & 1) * 0x1000];
}
