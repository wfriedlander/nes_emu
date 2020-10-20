#include "ppu.h"

#include "bus.h"

//#include <iostream>
//#include <algorithm>


PPU::PPU(Bus* bus) : mBus(bus)
{
    RegisterField("control", &mControlReg);
    RegisterField("mask", &mMaskReg);
    RegisterField("status", &mStatusReg);
    RegisterField("regt", &mRegT);
    RegisterField("regv", &mRegV);
    RegisterField("regx", &mRegX);
    RegisterField("regy", &mRegY);
    RegisterField("regw", &mRegW);
    RegisterField("cycle", &mCycle);
    RegisterField("end", &mFrameEnd);
    RegisterField("frame", &mFrame);
    RegisterField("line", &mLine);
    RegisterField("tick", &mTick);
    RegisterField("latch", &mLatch);
    RegisterField("write", &mFirstWrite);
    RegisterField("vaddr", &mVaddr);
    RegisterField("vbuffer", &mVbuffer);
    RegisterField("oamaddr", &mAddrOAM);
    RegisterField("oam", (byte*)&mOAM, 256);
    RegisterField("screen", (byte*)&mScreen, 240*256);
    RegisterField("sprites", (byte*)&mSprites, 256);
    RegisterField("palette", &mPalette);
    RegisterField("patternhigh", &mPatternHigh);
    RegisterField("patternlow", &mPatternLow);
    RegisterField("pattern", &mPattern);
    RegisterField("shift", &mShift);
}

void PPU::SetVideoBackend(IVideo* video)
{
    mVideo = video;
}

void PPU::RunUntil(nes_time cpu_time)
{
    nes_time ppu_time = cpu_time * 3;
    nes_time pending_ticks = ppu_time - mCycle;

    while (pending_ticks > 0) {
        nes_time remaining_in_line = 341 - mTick;
        nes_time to_run = std::min(pending_ticks, remaining_in_line);

        if (mLine < 240) {
            RenderLine(mTick, to_run);
        }
        else if (mLine < 261)
        {
            PostRender(mTick, to_run);
        }
        else if (mLine == 261)
        {
            PreRender(mTick, to_run);
        }

        pending_ticks -= to_run;
    }

    mCycle = ppu_time;
}

void PPU::RenderLine(nes_time start, nes_time ticks)
{
    if (mTick == 0 && ticks > 0)
        mTick += 1;

    // FIXME: BG/SPRITE MASKING IS REALLY WRONG
    if (mMaskReg.sb)
    {
        while (mTick > 0 && mTick <= 256 && mTick < start + ticks) {
            if (mTick == 1) {
                LoadSprites();
            }
            if (mTick == 1 || mShift == 7) {
                LoadTile();
            }
            byte color_index = (((mPatternHigh >> mShift) << 1) | ((mPatternLow >> mShift) & 1)) & 3;
            byte palette_index = color_index ? mPalette : 0;
            word color_addr = 0x3F00 | palette_index | color_index;
            byte bg_color = Read(color_addr) & 0x3F;
            bool bg_opaque = color_index > 0;

            word sprite = mSprites[mTick - 1];
            byte sprite_color = ByteLow(sprite);
            word sprite_opaque = sprite & 0x2000;
            word sprite_foreground = sprite & 0x4000;
            word sprite0 = sprite & 0x8000;

            if (sprite_opaque && (sprite_foreground || !bg_opaque)) {
                mScreen[mLine][mTick - 1] = sprite_color;
            }
            else {
                mScreen[mLine][mTick - 1] = bg_color;
            }

            if (sprite0 && sprite_opaque && bg_opaque) {
                mStatusReg.s = 1;
            }

            mShift = (mShift - 1) & 7;
            mTick += 1;
        }

        if (start + ticks == 341) {
            mRegV += 0x1000;
            word y = ((mRegV & 0x03E0) + (mRegV >> 10)) & 0x03E0;
            if (y == 0x3C0) {
                y = 0;
                mRegV ^= 0x0800;
            }
            mRegV = ((mRegV & 0xF800) | y | (mRegT & 0x41F)) & 0x7FFF;
            mShift = 7 - mRegX;
        }
    }

    mTick = start + ticks;
    if (mTick == 341) {
        mTick = 0;
        mLine += 1;
    }
}

void PPU::PostRender(nes_time start, nes_time ticks)
{
    mTick = start + ticks;

    if (mLine == 241 && start < 2 && mTick >= 2) {
        mVideo->VideoUpdate(mScreen);
        mStatusReg.v = 1;
        if (mControlReg.v) {
            mBus->TriggerNMI();
        }
    }

    if (mTick == 341) {
        mLine += 1;
        mTick = 0;
    }
}

void PPU::PreRender(nes_time start, nes_time ticks)
{
    mTick = start + ticks;

    mStatusReg = 0;
    if (mTick == 341) {
        mFrame += 1;
        mLine = 0;
        mTick = mFrame & 1;
        mBus->FrameComplete();
        if (mMaskReg.sb) {
            mRegV &= ~0x7BE0;
            mRegV |= (mRegT & 0x7BE0);
        }
        mShift = 7 - mRegX;
    }
}

void PPU::LoadTile()
{
    byte name = Read(0x2000 | (mRegV & 0xFFF));
    byte attr = Read(0x23C0 | (mRegV & 0x0C00) | ((mRegV >> 4) & 0x38) | ((mRegV >> 2) & 0x07));
    byte aty = (mRegV >> 5) & 0x2;
    byte atx = (mRegV >> 1) & 0x1;
    byte t = ((mRegV >> 5 & 0x20) | (mRegV & 0x1F)) + 1;
    mPalette = ((attr >> (2 * (aty | atx))) & 0x3) << 2;
    mPatternLow = Read((mControlReg.b << 12) | (name << 4) | (0 << 3) | ((mRegV >> 12) & 0x7));
    mPatternHigh = Read((mControlReg.b << 12) | (name << 4) | (1 << 3) | ((mRegV >> 12) & 0x7));
    mRegV = (mRegV & 0xFBE0) | ((t & 0x20) << 5) | (t & 0x1F);
}

void PPU::LoadSprites()
{
    for (int i = 0; i < 256; i++) mSprites[i] = 0;

    oam* primary = reinterpret_cast<oam*>(&mOAM);

    byte y_size = 8 << mControlReg.h;
    byte sprites = 0;

    for (byte i = 0; i < 64; i++)
    {
        auto& sprite = primary[i];

        if (!(sprite.y < mLine && (sprite.y + y_size) >= mLine)) {
            continue;
        }

        if (sprites++ == 8) {
            mStatusReg.o = 1;
            return;
        }

        byte ypx = 0;
        word p = 0;

        if (mControlReg.h)
        {
            auto page = sprite.t & 1;
            ypx = (mLine - sprite.y - 1) & 0xF;
            auto tile = (ypx > 7) ? ((sprite.t & 0xFE) + 1) : (sprite.t & 0xFE);
            ypx &= 0x07;
            if (sprite.a & 0x80)
            {
                tile ^= 0x01;
                ypx = 7 - ypx;
            }
            p = PatternTable(page, tile, ypx);
        }
        else
        {
            ypx = (mLine - sprite.y - 1) & 0x7;
            if (sprite.a & 0x80)
                ypx = 7 - ypx;
            p = PatternTable(mControlReg.s, sprite.t, ypx);
        }

        for (int px = 7; px >= 0; px--)
        {
            byte dx = sprite.a & 0x40 ? px : (7 - px);
            if (sprite.x + dx < 256)
            {
                byte color_index = (p >> (px * 2)) & 0x3;
                byte palette_index = (sprite.a & 0x3) << 2;
                word color_addr = 0x3F10 | palette_index | color_index;

                byte color = Read(color_addr) & 0x3F;
                word opaque = (color_index > 0) << 13;
                word foreground = (!(sprite.a & 0x20)) << 14;
                word sprite0 = ((i == 0) << 15) & (opaque << 2);

                if (opaque)
                    mSprites[sprite.x + dx] = ((mSprites[sprite.x + dx] & 0x8000) | sprite0) | foreground | opaque | color;
            }
        }
    }
}

void PPU::Reset()
{
    mCycle = 0;
    mLine = 0;
    mFrame = 0;
    mStatusReg = 0;
    mControlReg = 0;
    mMaskReg = 0;
    mLatch = 0;
    mFirstWrite = true;
}

nes_time PPU::MinTimeToNMI()
{
    nes_time nmi_time = 241 * 341 + 1;
    nes_time current_time = ((nes_time)(mLine - 1) * 341) + mTick;

    if (current_time < (nmi_time - 10)) {
        return (nmi_time - current_time) / 3;
    }
    else if (current_time > nmi_time) {
        nes_time remaining_time = (262 * 341) - current_time;
        return (nmi_time + remaining_time) / 3;
    }
    else {
        return 1;
    }
}

byte PPU::RegisterRead(nes_time cycle, word address)
{
    RunUntil(cycle);

    switch (address)
    {
    case 0: break;
    case 1: break;
    case 2:
        mLatch = mStatusReg | (mLatch & 0x1F);
        mStatusReg.v = 0;
        mFirstWrite = true;
        mRegW = 0;
        break;
    case 3: break;
    case 4:
        mLatch = mOAM[mAddrOAM];
        mAddrOAM += 1;
        break;
    case 5: break;
    case 6: break;
    case 7:
        mLatch = mVbuffer;
        mVbuffer = Read(mVaddr);
        if (mVaddr > 0x3EFF && mVaddr < 0x4000)
        {
            mLatch = mVbuffer;
            mVbuffer = Read(mVaddr & 0x2FFF);
        }
        mVaddr += mControlReg.i ? 32 : 1;


        mRegV += mControlReg.i ? 32 : 1;
        break;
    default: break;
    }
    return mLatch;
}

void PPU::RegisterWrite(nes_time cycle, word address, byte value)
{
    RunUntil(cycle);

    switch (address)
    {
    case 0:
        mControlReg = value;
        mRegT = (mRegT & ~0xC00) | ((value & 0x3) << 10);
        break;
    case 1: mMaskReg = value; break;
    case 2: break;
    case 3: mAddrOAM = value; break;
    case 4: mOAM[mAddrOAM] = value; break;
    case 5:
        if (!mRegW) {
            mRegX = value & 0x7;
            mRegT = (mRegT & 0xFFE0) | ((value & 0xF8) >> 3);
        }
        else {
            mRegT &= 0xC1F;
            mRegT |= ((value & 0xF8) << 2);
            mRegT |= ((value & 0x07) << 12);
        }
        mRegW = mRegW ^ 1;
        break;
    case 6:
        if (!mRegW) {
            mRegT &= 0xFF;
            mRegT |= (value & 0x3F) << 8;
        }
        else {
            mRegT &= 0xFF00;
            mRegT |= value;
            mRegV = mRegT;
        }
        mRegW = mRegW ^ 1;

        if (mFirstWrite)
            mVaddr = (value << 8) | (mVaddr & 0xFF);
        else
            mVaddr = (mVaddr & 0xFF00) | value;
        mFirstWrite = !mFirstWrite;
        break;
    case 7:
        Write(mVaddr, value);
        mVaddr += mControlReg.i ? 32 : 1;
        mRegV += mControlReg.i ? 32 : 1;
    default: break;
    }
}

void PPU::ActivateDMA(byte address)
{
    for (word i = 0; i < 256; i++)
    {
        mOAM[i] = mBus->CpuRead(Word(address, i & 0xFF));
    }
}

void PPU::Write(word address, byte value)
{
    mBus->PpuWrite(address, value);
}

byte PPU::Read(word address)
{
    mVaddr = address;
    return mBus->PpuRead(address);
}

word PPU::PatternTable(byte side, word tile, byte row)
{
    byte l = Read((side << 12) | (tile << 4) | (0 << 3) | row);
    byte h = Read((side << 12) | (tile << 4) | (1 << 3) | row);

    word pattern = 0;
    pattern |= (h & 0x80) << 8;
    pattern |= (h & 0x40) << 7;
    pattern |= (h & 0x20) << 6;
    pattern |= (h & 0x10) << 5;
    pattern |= (h & 0x08) << 4;
    pattern |= (h & 0x04) << 3;
    pattern |= (h & 0x02) << 2;
    pattern |= (h & 0x01) << 1;
    pattern |= (l & 0x80) << 7;
    pattern |= (l & 0x40) << 6;
    pattern |= (l & 0x20) << 5;
    pattern |= (l & 0x10) << 4;
    pattern |= (l & 0x08) << 3;
    pattern |= (l & 0x04) << 2;
    pattern |= (l & 0x02) << 1;
    pattern |= (l & 0x01) << 0;
    return pattern;
}
