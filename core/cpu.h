#pragma once

#include <cstdio>

#include "common.h"
#include "bus.h"


class Bus;

class CPU
{

public:
    CPU(Bus* bus) : mBus(bus) { fopen_s(&log, "log4.txt", "w"); }
    ~CPU() { fclose(log); }

public:
    void Reset();
    void RunUntil(nes_time time);
    void SetNMI(bool set) { mNMI = set; }
    void SetIRQ(bool set) { mIRQ = set; }
    void NMI();
    void IRQ();

public:
    nes_time CurrentCycle() { return mCycle; }

private:
    void Write(word address, byte value);
    byte Read(word address);

private:
    struct status
    {
        byte n = 0;     // Negative
        byte v = 0;     // Overflow
        byte b = 0;     // 1 on PHP/BRK, 0 on IRQ/NMI
        byte d = 0;     // Decimal
        byte i = 0;     // Interrupt Disable
        byte z = 0;     // Zero
        byte c = 0;     // Carry
        operator byte() const { return (n << 7 | v << 6 | 1 << 5 | b << 4 | d << 3 | i << 2 | z << 1 | c << 0); }
        void operator =(byte val) { n = (val >> 7) & 0x01; v = (val >> 6) & 0x01; b = 0; d = (val >> 3) & 0x01; i = (val >> 2) & 0x01; z = (val >> 1) & 0x01; c = (val >> 0) & 0x01; }
    };

    status sr;
    byte a = 0;
    byte x = 0;
    byte y = 0;
    byte sp = 0xFF;
    word pc = 0;

public:
    FILE* log = nullptr;
    Bus* mBus = nullptr;
    nes_time mCycle = 0;
    bool mNMI = false;
    bool mIRQ = false;

private:
    // Addressing Modes
    word Implied();
    word Immediate();
    word Zeropage();
    word Zeropage_Indexed(byte& reg);
    word Absolute();
    word Absolute_Indexed(byte& reg);
    word Absolute_Indexed_W(byte& reg);
    word Relative();
    word Indirect();
    word Indirect_X();
    word Indirect_Y();
    word Indirect_Y_W();

    // Instructions
    void ADC(word address);
    void AND(word address);
    void ASL(word address);
    void ASL_A();
    void BRANCH(word address, bool branch);
    void BIT(word address);
    void BRK(word address);
    void CLEAR(byte& flag);
    void COMPARE(word address, byte& reg);
    void DEC(word address);
    void DECREMENT(byte& reg);
    void EOR(word address);
    void INC(word address);
    void INCREMENT(byte& reg);
    void JMP(word address);
    void JSR();
    void LOAD(word address, byte& reg);
    void LSR(word address);
    void LSR_A();
    void NOP();
    void ORA(word address);
    void PHA();
    void PHP();
    void PLA();
    void PLP();
    void ROL(word address);
    void ROL_A();
    void ROR(word address);
    void ROR_A();
    void RTI();
    void RTS();
    void SBC(word address);
    void SET(byte& flag);
    void STORE(word address, byte& reg);
    void TRANSFER(byte& from, byte& to);
    void TXS();
};


