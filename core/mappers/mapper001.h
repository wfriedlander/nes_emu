#pragma once

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
    byte mReg[4];
    word mShift = 0x100;
};

