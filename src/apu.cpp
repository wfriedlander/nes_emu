#include "apu.h"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <QDebug>



///////////////////////////////////////////////////////////////////////////
// LENGTH COUNTER
////////////////////////////////////////////////////

void LengthCounter::SetEnabled(bool enabled)
{
    mEnabled = enabled;
    if (!mEnabled)
        mLength = 0;
}

void LengthCounter::SetHalted(bool halted)
{
    mHalted = halted;
}

void LengthCounter::SetLength(byte length)
{
    if (mEnabled)
        mLength = mLengthTable[length];
}

void LengthCounter::TickLengthCounter()
{
    if (!mHalted && mLength > 0)
        mLength -= 1;
}

bool LengthCounter::Silenced()
{
    return mLength == 0;
}


///////////////////////////////////////////////////////////////////////////
// ENVELOPE
////////////////////////////////////////////////////


void Envelope::SetEnvelope(byte value)
{
    mVolume = value;
}

void Envelope::SetConstantVolume(bool constant)
{
    mConstantVolume = constant;
}

void Envelope::SetLoopFlag(bool loop)
{
    mLoopFlag = loop;
}

void Envelope::SetStartFlag()
{
    mStartFlag = true;
}

void Envelope::TickEnvelope()
{
    if (mStartFlag)
    {
        mStartFlag = false;
        mDecayLevel = 15;
        mDivider = mVolume;
    }
    else if (--mDivider == 255)
    {
        mDivider = mVolume;
        if (--mDecayLevel == 255)
        {
            mDecayLevel = mLoopFlag ? 15 : 0;
        }
    }
}

byte Envelope::Volume()
{
    return mConstantVolume ? mVolume : mDecayLevel;
}



///////////////////////////////////////////////////////////////////////////
// PULSE
////////////////////////////////////////////////////

Pulse::Pulse(Blip_Buffer* buffer, byte channel) : mChannel(channel)
{
    mSynth.volume(0.1128 * 0.5);
    mSynth.output(buffer);
}

void Pulse::QuarterFrame()
{
    TickEnvelope();
}

void Pulse::HalfFrame()
{
    TickLengthCounter();

    if (--mSweep == 0xFF)
    {
        mSweep = mSweepPeriod;
        if (mSweepEnabled)
        {
            word new_period = mTimerReg;
            if (mSweepNegate)
                new_period -= (mTimerReg >> mSweepShift) - mChannel;
            else
                new_period += (mTimerReg >> mSweepShift);

            mSweepMuted = (new_period >= 0x800 || mTimerReg < 8);

            if (mSweepEnabled && !mSweepMuted)
                mTimerReg = new_period;
        }
    }

    if (mSweepReload)
    {
        mSweepReload = false;
        mSweep = mSweepPeriod;
    }
}

void Pulse::Process(word cycles)
{
    if (mSweepMuted || mTimerMuted || Silenced())
    {
        mSynth.update(0, 0);
        return;
    }

    blip_time_t time = 0;

    while (cycles > 0)
    {
        auto m = std::min(cycles, mTimer);
        cycles -= m;
        mTimer -= m;
        time += m;

        if (mTimer == 0)
        {
            mTimer = mTimerReg * 2;
            mSequencePos = (mSequencePos + 1) & 0x7;
            auto sample = mSequenceTable[mDuty][mSequencePos] ? Volume() : 0;
            mSynth.update(time, sample);
        }
    }
}

void Pulse::SetDuty(byte duty)
{
    mDuty = duty;
}

void Pulse::SetSweep(byte sweep)
{
    mSweepEnabled = sweep & 0x80;
    mSweepPeriod = (sweep >> 4) & 0x07;
    mSweepNegate = sweep & 0x08;
    mSweepShift = sweep & 0x07;
    mSweepReload = true;
}

void Pulse::SetTimerHigh(byte period)
{
    mTimerReg = Word(period, mTimerReg & 0xFF);
    mSequencePos = 0;
    mTimerMuted = (mTimerReg < 8);
}

void Pulse::SetTimerLow(byte period)
{
    mTimerReg = Word(mTimerReg >> 8, period);
    mTimerMuted = (mTimerReg < 8);
}





///////////////////////////////////////////////////////////////////////////
// TRIANGLE
////////////////////////////////////////////////////


Triangle::Triangle(Blip_Buffer* buffer)
{
    mSynth.volume(0.12765 * 0.5);
    mSynth.output(buffer);
}

void Triangle::QuarterFrame()
{
    if (mCounterReload)
    {
        mCounter = mCounterReg;
        mCounterReload = mControl;
    }
    else if (mCounter > 0)
    {
        mCounter -= 1;
    }
}

void Triangle::HalfFrame()
{
    TickLengthCounter();
}

void Triangle::Process(word cycles)
{
    if (!mCounter || Silenced())
    {
        mSynth.update(0, 0);
        return;
    }

    blip_time_t time = 0;

    while (cycles > 0)
    {
        auto m = std::min(cycles, mTimer);
        cycles -= m;
        mTimer -= m;
        time += m;

        if (mTimer == 0)
        {
            mTimer = mTimerReg;
            mSequencePos = (mSequencePos + 1) & 0x1F;
            auto sample = mSequenceTable[mSequencePos];
            mSynth.update(time, sample);
        }
    }
}

void Triangle::SetControlFlag(bool flag)
{
    mControl = flag;
}

void Triangle::SetCounter(byte value)
{
    mCounterReg = value;
}

void Triangle::SetTimerHigh(byte period)
{
    mTimerReg = Word(period, mTimerReg & 0xFF);
    mSequencePos = 0;
}

void Triangle::SetTimerLow(byte period)
{
    mTimerReg = Word(mTimerReg >> 8, period);
}

void Triangle::SetCounterReloadFlag()
{
    mCounterReload = true;
}



///////////////////////////////////////////////////////////////////////////
// NOISE
////////////////////////////////////////////////////


Noise::Noise(Blip_Buffer* buffer)
{
    mSynth.volume(0.0741 * 0.5);
    mSynth.output(buffer);
}

void Noise::QuarterFrame()
{
    TickEnvelope();
}

void Noise::HalfFrame()
{
    TickLengthCounter();
}

void Noise::Process(word cycles)
{
    if (Silenced())
    {
        word feedback = (mShift << 13) ^ (mShift << 14);
        mShift = (feedback & 0x4000) | (mShift >> 1);
        mSynth.update(0, 0);
        return;
    }

    blip_time_t time = 0;

    mSynth.update(0, mLast);

    while (cycles > 0)
    {
        auto m = std::min(cycles, mTimer);
        cycles -= m;
        mTimer -= m;
        time += m;

        if (mTimer == 0)
        {
            mTimer = mTimerReg;
            //word feedback = ((mShift ^ (mShift >> mShiftMode)) << 14) & 0x4000;
            word feedback = ((mShift << mShiftMode) ^ (mShift << 14)) & 0x4000;
            auto sample = (mShift & 1) ? Volume() : 0;
            //std::cout << "sample " << (int)mTimer << " " << (int)sample << " " << (int)Volume() << " " << (int)mShift << "\n";
            if (sample != mLast) {
                mSynth.update(time, sample);
                mLast = sample;
            }

            mShift = feedback | (mShift >> 1);
        }
    }
}

void Noise::SetMode(bool mode)
{
    mShiftMode = mode ? 8 : 13;
}

void Noise::SetPeriod(byte period)
{
    mTimerReg = mPeriodTable[period];
}



///////////////////////////////////////////////////////////////////////////
// DMC
////////////////////////////////////////////////////


DMC::DMC(Bus* bus, Blip_Buffer* buffer) : mBus(bus)
{
    mSynth.volume(0.42545 * 0.5);
    mSynth.output(buffer);
}


void DMC::Process(word cycles)
{
    blip_time_t time = 0;
    mSynth.update(0, mOutput);

    while (cycles > 0)
    {
        auto m = std::min(cycles, mTimer);
        cycles -= m;
        mTimer -= m;
        time += m;

        if (mTimer == 0)
        {
            mTimer = mTimerReg;

            if (!mSilenced) {
                bool add = mShiftReg & 0x01;
                if (add && mOutput < 126) {
                    mOutput += 2;
                }
                else if (!add && mOutput > 1) {
                    mOutput -= 2;
                }
            }


            mSynth.update(time, mOutput);

            mShiftReg = mShiftReg >> 1;

            if (--mShiftCount == 0) {
                mShiftCount = 8;

                if (mLoop && !mCurrentRemaining) {
                    mCurrentRemaining = mSamplesRemaining;
                    mCurrentAddress = mSampleAddress;
                }

                if (mCurrentRemaining > 0) {
                    mSilenced = false;
                    mShiftReg = mBus->CpuRead(mCurrentAddress);
                    mCurrentAddress = (mCurrentAddress + 1) | 0x8000;
                    mCurrentRemaining -= 1;
                }
                else {
                    mSilenced = true;
                }
            }
        }
    }
}

void DMC::SetEnabled(bool enabled)
{
    if (enabled && !mCurrentRemaining) {
        mCurrentRemaining = mSamplesRemaining;
        mCurrentAddress = mSampleAddress;
    }
    else if (!enabled) {
        mCurrentRemaining = 0;
    }
}

void DMC::SetIRQ(bool irq)
{

}

void DMC::SetLoop(bool loop)
{
    mLoop = loop;
}

void DMC::SetRate(byte rate)
{
    mTimerReg = mPeriodTable[rate];
}

void DMC::SetSample(byte sample)
{
    mOutput = sample;
}

void DMC::SetAddress(byte address)
{
    mSampleAddress = 0xC000 | (address << 6);
}

void DMC::SetLength(byte length)
{
    mSamplesRemaining = (length << 4) | 1;
}



///////////////////////////////////////////////////////////////////////////
// APU
////////////////////////////////////////////////////



APU::APU(Bus* bus) : mBus(bus), mPulse0(&mBuffer, 0), mPulse1(&mBuffer, 1), mTriangle(&mBuffer), mNoise(&mBuffer), mDMC(bus, &mBuffer)
{
//    mBuffer.clock_rate(1789773);
//    mBuffer.set_sample_rate(48000);
}

void APU::SetAudioBackend(IAudio* audio)
{
    audio->SetBuffer(&mBuffer);
}

void APU::EnableLogging(bool enable)
{
    mLogging = enable;
}

void APU::Execute(word cycles)
{
    mCycle += cycles;

    while (mCycle >= mQuarterFrame)
    {
        ProcessSamples(mQuarterFrame - mProcessed);

        if (mFrameReg.m && mFrameCount == 2)
        {
            mQuarterFrame += 7457;
            mHalfFrame += 7457;
        }

        mQuarterFrame += 7457;
        QuarterFrame();

        if (mCycle >= mHalfFrame)
        {
            mHalfFrame += 14914;
            HalfFrame();
        }

        if (++mFrameCount == 4)
        {
            mFrameCount = 0;
            //if (!(mFrameReg.m | mFrameReg.i))
            //	mBus->TriggerIRQ();
        }
    }
}

void APU::QuarterFrame()
{
    mPulse0.QuarterFrame();
    mPulse1.QuarterFrame();
    mTriangle.QuarterFrame();
    mNoise.QuarterFrame();
}

void APU::HalfFrame()
{
    mPulse0.HalfFrame();
    mPulse1.HalfFrame();
    mTriangle.HalfFrame();
    mNoise.HalfFrame();
}

void APU::FrameDone()
{
//    qDebug() << "samples processed" << mCycle << mCycle - mLastProcessed << mCycle - mProcessed;
    mLastProcessed = mCycle;
}

void APU::Reset()
{
    RegisterWrite(0x15, 0);
    RegisterWrite(0x17, 0);
}

byte APU::RegisterRead(word address)
{
    return 0;
}

void APU::RegisterWrite(word address, byte value)
{
    ProcessSamples(mCycle - mProcessed);

    switch (address)
    {
        // PULSE 0 REGISTERS
    case 0x00:
        mPulse0.SetDuty(value >> 6);
        mPulse0.SetHalted(value & 0x20);
        mPulse0.SetLoopFlag(value & 0x20);
        mPulse0.SetConstantVolume(value & 0x10);
        mPulse0.SetEnvelope(value & 0x0F);
        break;
    case 0x01:
        mPulse0.SetSweep(value);
        break;
    case 0x02:
        mPulse0.SetTimerLow(value);
        break;
    case 0x03:
        mPulse0.SetTimerHigh(value & 0x07);
        mPulse0.SetLength(value >> 3);
        mPulse0.SetStartFlag();
        break;

        // PULSE 1 REGISTERS
    case 0x04:
        mPulse1.SetDuty(value >> 6);
        mPulse1.SetHalted(value & 0x20);
        mPulse1.SetLoopFlag(value & 0x20);
        mPulse1.SetConstantVolume(value & 0x10);
        mPulse1.SetEnvelope(value & 0x0F);
        break;
    case 0x05:
        mPulse1.SetSweep(value);
        break;
    case 0x06:
        mPulse1.SetTimerLow(value);
        break;
    case 0x07:
        mPulse1.SetTimerHigh(value & 0x07);
        mPulse1.SetLength(value >> 3);
        mPulse1.SetStartFlag();
        break;

        // TRIANGLE REGISTERS
    case 0x08:
        mTriangle.SetControlFlag(value & 0x80);
        mTriangle.SetHalted(value & 0x80);
        mTriangle.SetCounter(value & 0x7F);
        break;
    case 0x09:
        break;
    case 0x0A:
        mTriangle.SetTimerLow(value);
        break;
    case 0x0B:
        mTriangle.SetTimerHigh(value & 0x07);
        mTriangle.SetLength(value >> 3);
        mTriangle.SetCounterReloadFlag();
        break;

        // NOISE REGISTERS
    case 0x0C:
        mNoise.SetHalted(value & 0x20);
        mNoise.SetLoopFlag(value & 0x20);
        mNoise.SetConstantVolume(value & 0x10);
        mNoise.SetEnvelope(value & 0x0F);
        break;
    case 0x0D:
        break;
    case 0x0E:
        mNoise.SetMode(value & 0x80);
        mNoise.SetPeriod(value & 0x0F);
        break;
    case 0x0F:
        mNoise.SetLength(value >> 3);
        mNoise.SetStartFlag();
        break;

        // DMC REGISTERS
    case 0x10:
        mDMC.SetIRQ(value & 0x80);
        mDMC.SetLoop(value & 0x40);
        mDMC.SetRate(value & 0x0F);
        break;
    case 0x11:
        mDMC.SetSample(value & 0x7F);
        break;
    case 0x12:
        mDMC.SetAddress(value);
        break;
    case 0x13:
        mDMC.SetLength(value);
        break;

        // STATUS REGISTER
    case 0x15:
        mPulse0.SetEnabled(value & 0x01);
        mPulse1.SetEnabled(value & 0x02);
        mTriangle.SetEnabled(value & 0x04);
        mNoise.SetEnabled(value & 0x08);
        mDMC.SetEnabled(value & 0x10);
        break;

        // FRAME COUNTER REGISTER
    case 0x17:
        mFrameReg = value;
        mFrameCount = 0;
        //mQuarterFrame = mCycle + 2;
        //mHalfFrame = mCycle + 2;
        //if (!mFrameReg.m)
        //{
        //	mQuarterFrame += 7457;
        //	mHalfFrame += 14914;
        //}
        break;
    default:
        break;
    }
}

void APU::ProcessSamples(word cycles)
{
    mPulse0.Process(cycles);
    mPulse1.Process(cycles);
    mTriangle.Process(cycles);
    mNoise.Process(cycles);
    mDMC.Process(cycles);
    mBuffer.end_frame(cycles);
    mProcessed += cycles;
}



