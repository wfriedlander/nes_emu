
#include "mapper001.h"

#include <iostream>

Mapper001::Mapper001(Cartridge& cart) : Mapper(cart)
{
    // LOAD SAVE DATA HERE
    MapRegion(SAVE_RAM, 8, sram);
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
        mShift = (value << 8) | ((mShift >> 1) & 0xFF);

        if (mShift & 0x8000) {
            mShift = ((mReg[0] | 0x0C) << 3) | 1;
            address = 0x8000;
        }

        if (mShift & 1) {
            mReg[(address >> 13) & 0x03] = (mShift >> 4) & 0xFF;
            mShift = 0x20;
            RegistersChanged();
        }
    }
    else if (address & 0x6000)
    {
        if (!(mReg[3] & 0x80)) {
            prg_map[SAVE_RAM][address & 0x1FFF] = value;
        }
    }
}

void Mapper001::RegistersChanged()
{
    switch (mReg[0] & 3)
    {
    case 0: MapNametable(SINGLE_LOW); break;
    case 1: MapNametable(SINGLE_HIGH); break;
    case 2: MapNametable(VERTICAL); break;
    case 3: MapNametable(HORIZONTAL); break;
    }

    switch ((mReg[0] >> 2) & 3)
    {
    case 0:
    case 1:
        MapRegion(PRG_0, 32, prg, (mReg[3] >> 1) & 0x7, 32);
        break;
    case 2:
        MapRegion(PRG_0, 16, prg);
        MapRegion(PRG_1, 16, prg, mReg[3] & 0xF);
        break;
    case 3:
        MapRegion(PRG_0, 16, prg, mReg[3] & 0xF);
        MapRegion(PRG_1, 16, prg, -1);
        break;
    }

    switch (mReg[0] >> 4)
    {
    case 0:
        MapRegion(CHR_0, 8, chr, mReg[1] >> 1);
        break;
    case 1:
        MapRegion(CHR_0, 4, chr, mReg[1], 4);
        MapRegion(CHR_1, 4, chr, mReg[2], 4);
        break;
    }
}

