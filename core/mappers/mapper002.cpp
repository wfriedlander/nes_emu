#include "mapper002.h"


namespace Core {


Mapper002::Mapper002(Cartridge& cart) : Mapper(cart)
{
    RegisterField("reg", &mReg);

    MapRegion(PRG_1, 16, prg, -1);
    MapRegion(CHR_0, 8, chr);
    MapNametable(mMirroring ? VERTICAL : HORIZONTAL);
    ApplyMapping();
}

void Mapper002::CpuWrite(word address, byte value)
{
    if (address & 0x8000)
    {
        mReg = value;
        ApplyMapping();
    }
}

void Mapper002::ApplyMapping()
{
    MapRegion(PRG_0, 16, prg, mReg);
}


}
