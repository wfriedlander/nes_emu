
#include "mapper001.h"

#include <iostream>

Mapper001::Mapper001(Cartridge& cart) : Mapper(cart)
{
    // LOAD SAVE DATA HERE

    MapPrg(1, 2, &sram[0]);
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
            sram[address & 0x1FFF] = value;
        }
    }
}

void Mapper001::RegistersChanged()
{
    switch (mReg[0] & 3)
    {
    case 0: MapName(0, 1, &ram[0]); MapName(1, 1, &ram[0]); MapName(2, 1, &ram[0]); MapName(3, 1, &ram[0]); break;
    case 1: MapName(0, 1, &ram[1024]); MapName(1, 1, &ram[1024]); MapName(2, 1, &ram[1024]); MapName(3, 1, &ram[1024]); break;
    case 2: MapName(0, 2, &ram[0]); MapName(2, 2, &ram[0]); break;
    case 3: MapName(0, 1, &ram[0]); MapName(1, 1, &ram[0]); MapName(2, 1, &ram[1024]); MapName(3, 1, &ram[1024]); break;
    }

    switch ((mReg[0] >> 2) & 3)
    {
    case 0:
    case 1: MapPrg(2, 4, PrgBank(((mReg[3] >> 1) & 7))); break;
    case 2: MapPrg(2, 2, PrgBank(0)); MapPrg(4, 2, PrgBank((mReg[3] & 0xF))); break;
    case 3: MapPrg(2, 2, PrgBank((mReg[3] & 0xF))); MapPrg(4, 2, PrgBank(-1)); break;
    }

    switch (mReg[0] >> 4)
    {
    case 0: MapChr(0, 8, ChrBank(mReg[1] >> 1)); break;
    case 1: MapChr(0, 4, ChrHalfBank(mReg[1])); MapChr(4, 4, ChrHalfBank(mReg[2])); break;
    }


}

