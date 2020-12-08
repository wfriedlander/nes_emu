#pragma once

#include "mapper.h"


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
    byte mReg[4] = {0x0C, 0x00, 0x00, 0x00};
    word mShift = 0x100;
};

