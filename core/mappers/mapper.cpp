
#include "mapper.h"

#include <iostream>


namespace Core {


Mapper::Mapper(Cartridge& cart) : ram(2048), palette(32), unmapped(1024)
{
    mName = cart.name;
    mBattery = cart.battery;
    mChrRam = cart.vram_size > 0;
    mMirroring = cart.mirroring;

    trainer = cart.trainer;
    prg = cart.prg;
    chr = cart.chr;

    chr.resize((cart.chr_size + cart.vram_size) * 1024);
    wram.resize(cart.wram_size * 1024);
    sram.resize(cart.sram_size * 1024);

    MapRegion(CPU_LOW, 48, unmapped);
    MapRegion(CHR_0, 8, unmapped);
    MapRegion(NAME_0, 4, unmapped);

    if (mBattery && sram.size() > 0) {
        std::ifstream st("Saves/" + mName + ".sav", std::ofstream::binary);
        st.read((char*)&sram[0], sram.size());
    }

    RegisterField("mirroring", &mMirroring);
    if (cart.vram_size > 0) {
        RegisterField("chr", &chr[0], chr.size());
    }
    if (sram.size() > 0) {
        RegisterField("sram", &sram[0], sram.size());
    }
    RegisterField("ram", &ram[0], 2048);
    RegisterField("palette", &palette[0], 32);

}

Mapper::~Mapper()
{
    if (mBattery && sram.size() > 0) {
        std::ofstream st("Saves/" + mName + ".sav", std::ofstream::binary);
        for (auto& b : sram) {
            st << b;
        }
    }
}

void Mapper::Deserialize(json& state)
{
    Serializable::Deserialize(state);
    ApplyMapping();
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


void Mapper::MapNametable(MIRRORING mirroring)
{
    switch (mirroring) {
    case MIRRORING::SINGLE_LOW:
        MapRegion(NAME_0, 1, ram, 0);
        MapRegion(NAME_1, 1, ram, 0);
        MapRegion(NAME_2, 1, ram, 0);
        MapRegion(NAME_3, 1, ram, 0);
        break;
    case MIRRORING::SINGLE_HIGH:
        MapRegion(NAME_0, 1, ram, 1);
        MapRegion(NAME_1, 1, ram, 1);
        MapRegion(NAME_2, 1, ram, 1);
        MapRegion(NAME_3, 1, ram, 1);
        break;
    case MIRRORING::VERTICAL:
        MapRegion(NAME_0, 2, ram, 0);
        MapRegion(NAME_2, 2, ram, 0);
        break;
    case MIRRORING::HORIZONTAL:
        MapRegion(NAME_0, 1, ram, 0);
        MapRegion(NAME_1, 1, ram, 0);
        MapRegion(NAME_2, 1, ram, 1);
        MapRegion(NAME_3, 1, ram, 1);
        break;
    }
}

std::string region_map(int region)
{
    switch (region) {
    case 0: return "CPU_LOW ";
    case 1: return "SAVE_RAM ";
    case 2: return "PRG_0 ";
    case 3: return "PRG_0B ";
    case 4: return "PRG_1 ";
    case 5: return "PRG_1B ";
    case 6: return "CHR_0 ";
    case 7: return "CHR_0B ";
    case 8: return "CHR_0C ";
    case 9: return "CHR_0D ";
    case 10: return "CHR_1 ";
    case 11: return "CHR_1B ";
    case 12: return "CHR_1C ";
    case 13: return "CHR_1D ";
    case 14: return "NAME_0 ";
    case 15: return "NAME_1 ";
    case 16: return "NAME_2 ";
    case 17: return "NAME_3 ";
    }
    return "";
}


void Mapper::MapRegion(REGION region, int kb_size, std::vector<byte>& mem, int bank, int kb_bank_size)
{
    byte** table = nullptr;
    int slice = 0;
    int slice_size = 0;
    int bank_size = 0;

    if (mem.size() == 0)
        return;

    if (region <= PRG_1) {
        table = prg_map;
        slice_size = 8;
        slice = region - CPU_LOW;
        bank_size = 16;
    } else if (region <= CHR_1) {
        table = chr_map;
        slice_size = 1;
        slice = region - CHR_0;
        bank_size = 8;
    }
    else {
        table = name_map;
        slice = region - NAME_0;
        slice_size = 1;
        bank_size = 1;
    }

    int slices = kb_size / slice_size;

    bank_size = ((int)mem.size() < (bank_size * 1024)) ? (mem.size() / 1024) : bank_size;
    bank_size = kb_bank_size < 0 ? bank_size : kb_bank_size;
    int banks = mem.size() / (bank_size * 1024);
    int adjusted_bank = (banks + bank % banks) & (banks - 1);

    int offset = adjusted_bank * bank_size * 1024;

    for (int i = 0; i < slices; i++) {
        table[slice + i] = &mem[offset];
//        std::cout << "MapRegion " << region_map(region + i) << offset << std::endl;
        offset = (offset + (slice_size * 1024)) % mem.size();
    }
}



}
