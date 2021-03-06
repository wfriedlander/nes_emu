#pragma once

#include "mapper.h"

namespace Core {


class Mapper001 : public Mapper
{
public:
    Mapper001(Cartridge& cart);

public:
    void CpuWrite(word address, byte value);

protected:
    void ApplyMapping();

private:
    byte mReg[4] = {0x0C, 0x00, 0x00, 0x00};
    word mShift = 0x20;
};


}
