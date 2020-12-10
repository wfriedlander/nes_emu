#include "cpu.h"
#include "bus.h"

namespace Core {


CPU::CPU(Bus* bus) : mBus(bus)
{
    RegisterField("sr", &sr);
    RegisterField("a", &a);
    RegisterField("x", &x);
    RegisterField("y", &y);
    RegisterField("sp", &sp);
    RegisterField("pc", &pc);
    RegisterField("cycle", &mCycle);
    RegisterField("nmi", &mNMI);
    RegisterField("irq", &mIRQ);
}


void CPU::Reset()
{
    sr.b = 0;
    sp--;
    sp--;
    sp--;
    pc = Word(Read(0xFFFD), Read(0xFFFC));
    sr.i = 1;
    mCycle = 0;
}

void CPU::NMI()
{
    sr.b = 0;
    auto [h, l] = Bytes(pc);
    Write(Word(0x01, sp--), h);
    Write(Word(0x01, sp--), l);
    Write(Word(0x01, sp--), sr);
    pc = Word(Read(0xFFFB), Read(0xFFFA));
    sr.i = 1;
}

void CPU::IRQ()
{
    sr.b = 0;
    auto [h, l] = Bytes(pc);
    Write(Word(0x01, sp--), h);
    Write(Word(0x01, sp--), l);
    Write(Word(0x01, sp--), sr);
    pc = Word(Read(0xFFFF), Read(0xFFFE));
    sr.i = 1;
}

void CPU::Write(word address, byte value)
{
    mCycle++;
    mBus->CpuWrite(address, value);
}

byte CPU::Read(word address)
{
    mCycle++;
    return mBus->CpuRead(address);
}

void CPU::RunUntil(nes_time time)
{
    if (mNMI) {
        NMI();
        mNMI = false;
    }

    if (mIRQ && !sr.i) {
        IRQ();
    }

    while (mCycle < time)
    {
        byte opcode = Read(pc);
//        byte status = sr;
//        fprintf(log, " PC  OP AC XR YR SR SP  N V - B D I Z C\n");
//        fprintf(log, "%04X %02X %02X %02X %02X %02X %02X  %d %d %d %d %d %d %d %d\n\n",
//            pc, opcode, a, x, y, status, sp, sr.n, sr.v, 1, sr.b, sr.d, sr.i, sr.z, sr.c);
        pc++;

        switch (opcode)
        {
        case 0x00: BRK(Immediate()); break;
        case 0x01: ORA(Indirect_X()); break;
        case 0x05: ORA(Zeropage()); break;
        case 0x06: ASL(Zeropage()); break;
        case 0x08: PHP(); break;
        case 0x09: ORA(Immediate()); break;
        case 0x0A: ASL_A(); break;
        case 0x0D: ORA(Absolute()); break;
        case 0x0E: ASL(Absolute()); break;
        case 0x10: BRANCH(Relative(), !sr.n); break;
        case 0x11: ORA(Indirect_Y()); break;
        case 0x15: ORA(Zeropage_Indexed(x)); break;
        case 0x16: ASL(Zeropage_Indexed(x)); break;
        case 0x18: CLEAR(sr.c); break;
        case 0x19: ORA(Absolute_Indexed(y)); break;
        case 0x1D: ORA(Absolute_Indexed(x)); break;
        case 0x1E: ASL(Absolute_Indexed_W(x)); break;
        case 0x20: JSR(); break;
        case 0x21: AND(Indirect_X()); break;
        case 0x24: BIT(Zeropage()); break;
        case 0x25: AND(Zeropage()); break;
        case 0x26: ROL(Zeropage()); break;
        case 0x28: PLP(); break;
        case 0x29: AND(Immediate()); break;
        case 0x2A: ROL_A(); break;
        case 0x2C: BIT(Absolute()); break;
        case 0x2D: AND(Absolute()); break;
        case 0x2E: ROL(Absolute()); break;
        case 0x30: BRANCH(Relative(), sr.n); break;
        case 0x31: AND(Indirect_Y()); break;
        case 0x35: AND(Zeropage_Indexed(x)); break;
        case 0x36: ROL(Zeropage_Indexed(x)); break;
        case 0x38: SET(sr.c); break;
        case 0x39: AND(Absolute_Indexed(y)); break;
        case 0x3D: AND(Absolute_Indexed(x)); break;
        case 0x3E: ROL(Absolute_Indexed_W(x)); break;
        case 0x40: RTI(); break;
        case 0x41: EOR(Indirect_X()); break;
        case 0x45: EOR(Zeropage()); break;
        case 0x46: LSR(Zeropage()); break;
        case 0x48: PHA(); break;
        case 0x49: EOR(Immediate()); break;
        case 0x4A: LSR_A(); break;
        case 0x4C: JMP(Absolute()); break;
        case 0x4D: EOR(Absolute()); break;
        case 0x4E: LSR(Absolute()); break;
        case 0x50: BRANCH(Relative(), !sr.v); break;
        case 0x51: EOR(Indirect_Y()); break;
        case 0x55: EOR(Zeropage_Indexed(x)); break;
        case 0x56: LSR(Zeropage_Indexed(x)); break;
        case 0x58: CLEAR(sr.i); break;
        case 0x59: EOR(Absolute_Indexed(y)); break;
        case 0x5D: EOR(Absolute_Indexed(x)); break;
        case 0x5E: LSR(Absolute_Indexed_W(x)); break;
        case 0x60: RTS(); break;
        case 0x61: ADC(Indirect_X()); break;
        case 0x65: ADC(Zeropage()); break;
        case 0x66: ROR(Zeropage()); break;
        case 0x68: PLA(); break;
        case 0x69: ADC(Immediate()); break;
        case 0x6A: ROR_A(); break;
        case 0x6C: JMP(Indirect()); break;
        case 0x6D: ADC(Absolute()); break;
        case 0x6E: ROR(Absolute()); break;
        case 0x70: BRANCH(Relative(), sr.v); break;
        case 0x71: ADC(Indirect_Y()); break;
        case 0x75: ADC(Zeropage_Indexed(x)); break;
        case 0x76: ROR(Zeropage_Indexed(x)); break;
        case 0x78: SET(sr.i); break;
        case 0x79: ADC(Absolute_Indexed(y)); break;
        case 0x7D: ADC(Absolute_Indexed(x)); break;
        case 0x7E: ROR(Absolute_Indexed_W(x)); break;
        case 0x81: STORE(Indirect_X(), a); break;
        case 0x84: STORE(Zeropage(), y); break;
        case 0x85: STORE(Zeropage(), a); break;
        case 0x86: STORE(Zeropage(), x); break;
        case 0x88: DECREMENT(y); break;
        case 0x8A: TRANSFER(x, a); break;
        case 0x8C: STORE(Absolute(), y); break;
        case 0x8D: STORE(Absolute(), a); break;
        case 0x8E: STORE(Absolute(), x); break;
        case 0x90: BRANCH(Relative(), !sr.c); break;
        case 0x91: STORE(Indirect_Y_W(), a); break;
        case 0x94: STORE(Zeropage_Indexed(x), y); break;
        case 0x95: STORE(Zeropage_Indexed(x), a); break;
        case 0x96: STORE(Zeropage_Indexed(y), x); break;
        case 0x98: TRANSFER(y, a); break;
        case 0x99: STORE(Absolute_Indexed_W(y), a); break;
        case 0x9A: TXS(); break;
        case 0x9D: STORE(Absolute_Indexed_W(x), a); break;
        case 0xA0: LOAD(Immediate(), y); break;
        case 0xA1: LOAD(Indirect_X(), a); break;
        case 0xA2: LOAD(Immediate(), x); break;
        case 0xA4: LOAD(Zeropage(), y); break;
        case 0xA5: LOAD(Zeropage(), a); break;
        case 0xA6: LOAD(Zeropage(), x); break;
        case 0xA8: TRANSFER(a, y); break;
        case 0xA9: LOAD(Immediate(), a); break;
        case 0xAA: TRANSFER(a, x); break;
        case 0xAC: LOAD(Absolute(), y); break;
        case 0xAD: LOAD(Absolute(), a); break;
        case 0xAE: LOAD(Absolute(), x); break;
        case 0xB0: BRANCH(Relative(), sr.c); break;
        case 0xB1: LOAD(Indirect_Y(), a); break;
        case 0xB4: LOAD(Zeropage_Indexed(x), y); break;
        case 0xB5: LOAD(Zeropage_Indexed(x), a); break;
        case 0xB6: LOAD(Zeropage_Indexed(y), x); break;
        case 0xB8: CLEAR(sr.v); break;
        case 0xB9: LOAD(Absolute_Indexed(y), a); break;
        case 0xBA: TRANSFER(sp, x); break;
        case 0xBC: LOAD(Absolute_Indexed(x), y); break;
        case 0xBD: LOAD(Absolute_Indexed(x), a); break;
        case 0xBE: LOAD(Absolute_Indexed(y), x); break;
        case 0xC0: COMPARE(Immediate(), y); break;
        case 0xC1: COMPARE(Indirect_X(), a); break;
        case 0xC4: COMPARE(Zeropage(), y); break;
        case 0xC5: COMPARE(Zeropage(), a); break;
        case 0xC6: DEC(Zeropage()); break;
        case 0xC8: INCREMENT(y); break;
        case 0xC9: COMPARE(Immediate(), a); break;
        case 0xCA: DECREMENT(x); break;
        case 0xCC: COMPARE(Absolute(), y); break;
        case 0xCD: COMPARE(Absolute(), a); break;
        case 0xCE: DEC(Absolute()); break;
        case 0xD0: BRANCH(Relative(), !sr.z); break;
        case 0xD1: COMPARE(Indirect_Y(), a); break;
        case 0xD5: COMPARE(Zeropage_Indexed(x), a); break;
        case 0xD6: DEC(Zeropage_Indexed(x)); break;
        case 0xD8: CLEAR(sr.d); break;
        case 0xD9: COMPARE(Absolute_Indexed(y), a); break;
        case 0xDD: COMPARE(Absolute_Indexed(x), a); break;
        case 0xDE: DEC(Absolute_Indexed_W(x)); break;
        case 0xE0: COMPARE(Immediate(), x); break;
        case 0xE1: SBC(Indirect_X()); break;
        case 0xE4: COMPARE(Zeropage(), x); break;
        case 0xE5: SBC(Zeropage()); break;
        case 0xE6: INC(Zeropage()); break;
        case 0xE8: INCREMENT(x); break;
        case 0xE9: SBC(Immediate()); break;
        case 0xEC: COMPARE(Absolute(), x); break;
        case 0xED: SBC(Absolute()); break;
        case 0xEE: INC(Absolute()); break;
        case 0xF0: BRANCH(Relative(), sr.z); break;
        case 0xF1: SBC(Indirect_Y()); break;
        case 0xF5: SBC(Zeropage_Indexed(x)); break;
        case 0xF6: INC(Zeropage_Indexed(x)); break;
        case 0xF8: SET(sr.d); break;
        case 0xF9: SBC(Absolute_Indexed(y)); break;
        case 0xFD: SBC(Absolute_Indexed(x)); break;
        case 0xFE: INC(Absolute_Indexed_W(x)); break;
        default:   NOP(); break;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////////////
// ADDRESSING MODES
///////////////////////////////////////////////////////////////////

word CPU::Implied()
{
    return 0;
}

word CPU::Immediate()
{
    return pc++;
}

word CPU::Zeropage()
{
    return Read(pc++);
}

word CPU::Zeropage_Indexed(byte& reg)
{
    word address = Read(pc++);
    Read(address);
    return ByteLow(address + reg);
}

word CPU::Absolute()
{
    byte low = Read(pc++);
    byte high = Read(pc++);
    return Word(high, low);
}

word CPU::Absolute_Indexed(byte& reg)
{
    word address = Absolute() + reg;
    if (reg > ByteLow(address)) {
        Read(address);
    }
    return address;
}

word CPU::Absolute_Indexed_W(byte& reg)
{
    word address = Absolute() + reg;
    Read(address);
    return address;
}

word CPU::Relative()
{
    return pc++;
}

word CPU::Indirect()
{
    byte l0 = Read(pc++);
    byte h0 = Read(pc++);
    byte l1 = Read(Word(h0, l0));
    byte h1 = Read(Word(h0, Byte(l0 + 1)));
    return Word(h1, l1);
}

word CPU::Indirect_X()
{
    byte ptr = Read(pc++);
    Read(ptr);
    byte l0 = ptr + x;
    byte h0 = l0 + 1;
    byte l1 = Read(l0);
    byte h1 = Read(h0);
    return Word(h1, l1);
}

word CPU::Indirect_Y()
{
    byte l0 = Read(pc++);
    byte h0 = l0 + 1;
    byte l1 = Read(l0);
    byte h1 = Read(h0);
    word address = Word(h1, l1) + y;
    if (y > ByteLow(address)) {
        Read(address);
    }
    return address;
}

word CPU::Indirect_Y_W()
{
    byte l0 = Read(pc++);
    byte h0 = l0 + 1;
    byte l1 = Read(l0);
    byte h1 = Read(h0);
    word address = Word(h1, l1) + y;
    Read(address);
    return address;
}


///////////////////////////////////////////////////////////////////////////////////////
// INSTRUCTIONS
///////////////////////////////////////////////////////////////////

void CPU::ADC(word address)
{
    auto mem = Read(address);
    word result = a + mem + sr.c;
    sr.c = result > 255;
    sr.v = !((a ^ mem) & 0x80) && ((a ^ result) & 0x80);
    a = ByteLow(result);
    sr.z = a == 0;
    sr.n = a >> 7;
}

void CPU::AND(word address)
{
    a &= Read(address);
    sr.z = a == 0;
    sr.n = a >> 7;
}

void CPU::ASL(word address)
{
    byte m = Read(address);
    word result = m << 1;
    byte t = Byte(result);
    sr.c = result > 255;
    sr.z = t == 0;
    sr.n = t >> 7;
    Write(address, m);
    Write(address, t);
}

void CPU::ASL_A()
{
    Read(pc);
    word result = a << 1;
    a = Byte(result);
    sr.c = result > 255;
    sr.z = a == 0;
    sr.n = a >> 7;
}

void CPU::BRANCH(word address, bool branch)
{
    sbyte offset = Read(address);
    if (branch) {
        Read(pc);
        word new_address = pc + offset;
        if (ByteHigh(pc) != ByteHigh(new_address)) {
            Read(pc);
        }
        pc = new_address;
    }
}

void CPU::BIT(word address)
{
    auto m = Read(address);
    sr.z = (a & m) ? 0 : 1;
    sr.v = (m & 0x40) ? 1 : 0;
    sr.n = (m & 0x80) ? 1 : 0;
}

void CPU::BRK(word address)
{
    Read(address);
    sr.b = 1;
    auto [h, l] = Bytes(pc);
    Write(Word(0x01, sp--), h);
    Write(Word(0x01, sp--), l);
    Write(Word(0x01, sp--), sr);
    auto pcl = Read(0xFFFE);
    auto pch = Read(0xFFFF);
    pc = Word(pch, pcl);
    sr.i = 1;
}

inline void CPU::CLEAR(byte& flag)
{
    Read(pc);
    flag = 0;
}

void CPU::COMPARE(word address, byte& reg)
{
    byte m = Read(address);
    sr.c = reg >= m;
    sr.z = reg == m;
    sr.n = ((byte)(reg - m)) >> 7;
}

void CPU::DEC(word address)
{
    byte m = Read(address);
    byte t = m - 1;
    sr.z = t == 0;
    sr.n = t >> 7;
    Write(address, m);
    Write(address, t);
}

void CPU::DECREMENT(byte& reg)
{
    Read(pc);
    reg -= 1;
    sr.z = reg == 0;
    sr.n = reg >> 7;
}

void CPU::EOR(word address)
{
    a ^= Read(address);
    sr.z = a == 0;
    sr.n = a >> 7;
}

void CPU::INC(word address)
{
    byte m = Read(address);
    byte t = m + 1;
    sr.z = t == 0;
    sr.n = t >> 7;
    Write(address, m);
    Write(address, t);
}

void CPU::INCREMENT(byte& reg)
{
    Read(pc);
    reg += 1;
    sr.z = reg == 0;
    sr.n = reg >> 7;
}

void CPU::JMP(word address)
{
    pc = address;
}

void CPU::JSR()
{
    byte low = Read(pc++);
    Read(Word(0x01, sp));
    auto [h, l] = Bytes(pc);
    Write(Word(0x01, sp--), h);
    Write(Word(0x01, sp--), l);
    byte high = Read(pc);
    pc = Word(high, low);
}

inline void CPU::LOAD(word address, byte& reg)
{
    reg = Read(address);
    sr.z = reg == 0;
    sr.n = reg >> 7;
}

void CPU::LSR(word address)
{
    byte m = Read(address);
    byte t = m >> 1;
    sr.c = m & 0x01;
    sr.z = t == 0;
    sr.n = t >> 7;
    Write(address, m);
    Write(address, t);
}

void CPU::LSR_A()
{
    Read(pc);
    sr.c = a & 0x01;
    a = a >> 1;
    sr.z = a == 0;
    sr.n = a >> 7;
}

inline void CPU::NOP()
{
    Read(pc);
}

void CPU::ORA(word address)
{
    a |= Read(address);
    sr.z = a == 0;
    sr.n = a >> 7;
}

void CPU::PHA()
{
    Read(pc);
    Write(Word(0x01, sp--), a);
}

void CPU::PHP()
{
    Read(pc);
    sr.b = 1;
    Write(Word(0x01, sp--), sr);
}

void CPU::PLA()
{
    Read(pc);
    Read(Word(0x01, sp++));
    a = Read(Word(0x01, sp));
    sr.z = a == 0;
    sr.n = a >> 7;
}

void CPU::PLP()
{
    Read(pc);
    Read(Word(0x01, sp++));
    sr = Read(Word(0x01, sp));
}

void CPU::ROL(word address)
{
    byte m = Read(address);
    word result = (m << 1) | sr.c;
    byte t = Byte(result);
    sr.c = result >> 8;
    sr.z = t == 0;
    sr.n = t >> 7;
    Write(address, m);
    Write(address, t);
}

void CPU::ROL_A()
{
    Read(pc);
    word result = (a << 1) | sr.c;
    a = Byte(result);
    sr.c = result >> 8;
    sr.z = a == 0;
    sr.n = a >> 7;
}

void CPU::ROR(word address)
{
    byte m = Read(address);
    word result = Word(sr.c, m);
    byte t = Byte(result >> 1);
    sr.c = result & 0x01;
    sr.z = t == 0;
    sr.n = t >> 7;
    Write(address, m);
    Write(address, t);
}

void CPU::ROR_A()
{
    Read(pc);
    word result = Word(sr.c, a);
    a = Byte(result >> 1);
    sr.c = result & 0x01;
    sr.z = a == 0;
    sr.n = a >> 7;
}

void CPU::RTI()
{
    Read(pc);
    Read(Word(0x01, sp++));
    sr = Read(Word(0x01, sp++));
    byte l = Read(Word(0x01, sp++));
    byte h = Read(Word(0x01, sp));
    pc = Word(h, l);
}

void CPU::RTS()
{
    Read(pc);
    Read(Word(0x01, sp++));
    byte l = Read(Word(0x01, sp++));
    byte h = Read(Word(0x01, sp));
    pc = Word(h, l);
    Read(pc++);
}

void CPU::SBC(word address)
{
    byte m = Read(address);
    word result = a - m - (1 - sr.c);
    sr.c = result <= 255;
    sr.v = ((result ^ a) & 0x80) && ((m ^ a) & 0x80);
    a = Byte(result);
    sr.z = a == 0;
    sr.n = a >> 7;
}

inline void CPU::SET(byte& flag)
{
    Read(pc);
    flag = 1;
}

inline void CPU::STORE(word address, byte& reg)
{
    Write(address, reg);
}

void CPU::TRANSFER(byte& from, byte& to)
{
    Read(pc);
    to = from;
    sr.z = to == 0;
    sr.n = to >> 7;
}

void CPU::TXS()
{
    Read(pc);
    sp = x;
}



}
