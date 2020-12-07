#include "mappers.h"


#include <QDebug>


Mapper::Mapper(CartridgeConfig& config, std::ifstream& rom) : mPpuRam(2048), mPaletteRam(32)
{
	for (int i = 0; i < config.prg_size; i++)
	{
		mPrgBank.emplace_back();
		rom.read(mPrgBank.back(), 16384);
	}

	for (int i = 0; i < config.chr_size; i++)
	{
		mChrBank.emplace_back();
		rom.read(mChrBank.back(), 8192);
	}

    if (config.chr_size == 0) {
        mChrBank.emplace_back();
    }

	mMirroring = config.mirroring;
	mPersistent = config.persistent;
	mFourScreen = config.four_screen;

    RegisterField("ppuram", mPpuRam.data(), 2048);
    RegisterField("palette", mPaletteRam.data(), 32);
}


byte Mapper::PpuRead(word address)
{
	switch (address >> 12)
	{
	case 0: 
	case 1: return PatternRead(address & 0x1FFF);
	case 2: return NameRead((address >> 10) & 0x3, address & 0x03FF);
	case 3: if (address & 0x0100) return PaletteRead(address & 0x1F);
	default: return PpuRead(address - 0x1000);
	}
}

void Mapper::PpuWrite(word address, byte value)
{
	switch (address >> 12)
	{
	case 0: 
	case 1: 
		PatternWrite(address & 0x1FFF, value);
		break;
	case 2: 
		NameWrite((address >> 10) & 0x3, address & 0x03FF, value);
		break;
	case 3: 
        if (address & 0x0100) {
            PaletteWrite(address & 0x1F, value);
            break;
        }
	default:
		PpuWrite(address - 0x1000, value);
		break;
	}
}

byte Mapper::PaletteRead(word address)
{
	word adjusted_addr = address & 0x3 ? address : address & 0xF;
	return mPaletteRam[adjusted_addr];
}

void Mapper::PaletteWrite(word address, byte value)
{
	word adjusted_addr = address & 0x3 ? address : address & 0xF;
	mPaletteRam[adjusted_addr] = value;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////
/// MAPPER 00
//////////////////////////////////////////////////

byte Mapper00::CpuRead(word address)
{
	if (address & 0x8000)
	{
		auto bank = 0;
		if (mPrgBank.size() > 1 && (address & 0x4000))
			bank = 1;
		return mPrgBank[bank][address & 0x3FFF];
	}
    return 0;
}

void Mapper00::CpuWrite(word, byte)
{

}

byte Mapper00::PatternRead(word address)
{
	return mChrBank[0][address];
}

void Mapper00::PatternWrite(word address, byte value)
{
	mChrBank[0][address] = value;
}

byte Mapper00::NameRead(byte table, word address)
{
	word adjusted_addr = (mMirroring ? (table & 1) << 10 : (table & 2) << 9) | address;
	return mPpuRam[adjusted_addr];
}

void Mapper00::NameWrite(byte table, word address, byte value)
{
	word adjusted_addr = (mMirroring ? (table & 1) << 10 : (table & 2) << 9) | address;
	mPpuRam[adjusted_addr] = value;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////
/// MAPPER 01
//////////////////////////////////////////////////


Mapper01::Mapper01(CartridgeConfig& config, std::ifstream& rom) : Mapper(config, rom)
{
    mPrgBankMode = 3;
    RegistersChanged();
}


byte Mapper01::CpuRead(word address)
{
    if (address & 0x8000)
    {
        auto bank = (address & 0x4000) ? mPrgBank1 : mPrgBank0;
        return mPrgBank[bank][address & 0x3FFF];
    }
    else if (address & 0x6000)
    {
        return mPrgRam[address & 0x1FFF];
    }
    return 0;
}

void Mapper01::CpuWrite(word address, byte value)
{
    if (address & 0x8000)
    {
        if (value & 0x80)
        {
            mShiftReg = 0;
            mShiftCount = 0;
            mPrgBankMode = 3;
            RegistersChanged();
        }
        else
        {
            mShiftReg |= ((value & 0x01) << mShiftCount);
            if (++mShiftCount == 5)
            {
                auto reg = (address >> 13) & 0x03;
                switch (reg)
                {
                case 0:
                    mChrBankMode = mShiftReg >> 4;
                    mPrgBankMode = (mShiftReg >> 2) & 3;
                    mMirroring = mShiftReg & 3;
                    break;
                case 1:
                    mChrControl0 = (mShiftReg & 0x1F) & (2 * mChrBank.size() - 1);
                    break;
                case 2:
                    mChrControl1 = (mShiftReg & 0x1F) & (2 * mChrBank.size() - 1);
                    break;
                case 3:
                    mPrgRamEnabled = mShiftReg & 0x10;
                    mPrgControl = (mShiftReg & 0x0F) & (mPrgBank.size() - 1);
                    break;
                }
                RegistersChanged();

//                if (reg == 0 || reg == 3)
//                    qDebug() << "write" << reg << mShiftReg << mPrgBankMode << mPrgControl << mPrgBank0 << mPrgBank1;

                mShiftReg = 0;
                mShiftCount = 0;



            }
        }
    }
    else if (address & 0x6000)
    {
        mPrgRam[address & 0x1FFF] = value;
    }
}

byte Mapper01::PatternRead(word address)
{
    auto bank = (address & 0x1000) ? mChrBank1 : mChrBank0;
    return bank[address & 0xFFF];
}

void Mapper01::PatternWrite(word address, byte value)
{
    auto bank = (address & 0x1000) ? mChrBank1 : mChrBank0;
    bank[address & 0xFFF] = value;
}

byte Mapper01::NameRead(byte table, word address)
{
    switch (mMirroring)
    {
    case 0:
        return mPpuRam[address];
    case 1:
        return mPpuRam[0x400 | address];
    case 2:
        return mPpuRam[((table & 1) << 10) | address];
    case 3:
        return mPpuRam[((table & 2) << 9) | address];
    default:
        return 0;
    }
}

void Mapper01::NameWrite(byte table, word address, byte value)
{
    switch (mMirroring)
    {
    case 0:
        mPpuRam[address] = value;
        break;
    case 1:
        mPpuRam[0x400 | address] = value;
        break;
    case 2:
        mPpuRam[((table & 1) << 10) | address] = value;
        break;
    case 3:
        mPpuRam[((table & 2) << 9) | address] = value;
        break;
    }
}

void Mapper01::RegistersChanged()
{
    if (mChrBankMode)
    {
        auto half0 = (mChrControl0 & 1) ? 4096 : 0;
        mChrBank0 = reinterpret_cast<byte*>(&mChrBank[mChrControl0 >> 1][half0]);
        auto half1 = (mChrControl1 & 1) ? 4096 : 0;
        mChrBank1 = reinterpret_cast<byte*>(&mChrBank[mChrControl1 >> 1][half1]);
    }
    else
    {
        mChrBank0 = reinterpret_cast<byte*>(&mChrBank[mChrControl0 >> 1][0]);
        mChrBank1 = reinterpret_cast<byte*>(&mChrBank[mChrControl0 >> 1][4096]);
    }

    if (mPrgBankMode == 3)
    {
        mPrgBank0 = mPrgControl;
        mPrgBank1 = mPrgBank.size() - 1;
    }
    else if (mPrgBankMode== 2)
    {
        mPrgBank0 = 0;
        mPrgBank1 = mPrgControl;
    }
    else
    {
        mPrgBank0 = mPrgControl >> 1;
        mPrgBank1 = (mPrgBank0 + 1) & (mPrgBank.size() - 1);
    }
}
