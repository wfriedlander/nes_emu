#pragma once

#include "mapper.h"

namespace Core {


class Mapper002 : public Mapper
{
public:
    Mapper002(Cartridge& cart);

public:
    void CpuWrite(word address, byte value);

protected:
    void ApplyMapping();

private:
    byte mReg = 0;
};


}
