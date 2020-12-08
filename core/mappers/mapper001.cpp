
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
        mShift = (mShift << 1) | (value & 0x81);

        if (mShift & 0x80) {
            mShift = 0x200C | mReg[0];
            address = 0x8000;
        }

        if (mShift & 0x2000) {
            mReg[(address >> 13) & 0x03] = mShift & 0xFF;
            std::cout << "reg write " << (int)address << ((address >> 13) & 0x03) << " " << (int)(mShift & 0xFF) << "\n";
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

