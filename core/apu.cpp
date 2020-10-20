#include "apu.h"
#include "bus.h"


///////////////////////////////////////////////////////////////////////////
// LENGTH COUNTER
////////////////////////////////////////////////////

LengthCounter::LengthCounter()
{
    RegisterField("enabled", &mEnabled);
    RegisterField("halted", &mHalted);
    RegisterField("length", &mLength);
}

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

void LengthCounter::Tick()
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

Envelope::Envelope()
{
    RegisterField("loop", &mLoopFlag);
    RegisterField("start", &mStartFlag);
    RegisterField("constant", &mConstantVolume);
    RegisterField("decay", &mDecayLevel);
    RegisterField("divider", &mDivider);
    RegisterField("volume", &mVolume);
}


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

void Envelope::Tick()
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

    RegisterField("length", &mLengthCounter);
    RegisterField("envelope", &mEnvelope);
    RegisterField("timer", &mTimer);
    RegisterField("timerreg", &mTimerReg);
    RegisterField("timermute", &mTimerMuted);
    RegisterField("reload", &mSweepReload);
    RegisterField("enabled", &mSweepEnabled);
    RegisterField("negate", &mSweepNegate);
    RegisterField("muted", &mSweepMuted);
    RegisterField("sweep", &mSweep);
    RegisterField("period", &mSweepPeriod);
    RegisterField("shift", &mSweepShift);
    RegisterField("channel", &mChannel);
    RegisterField("duty", &mDuty);
    RegisterField("sequence", &mSequencePos);
}

void Pulse::SetEnabled(bool enabled)
{
    mLengthCounter.SetEnabled(enabled);
}

void Pulse::WriteReg0(byte value)
{
    mDuty = value >> 6;
    mLengthCounter.SetHalted(value & 0x20);
    mEnvelope.SetLoopFlag(value & 0x20);
    mEnvelope.SetConstantVolume(value & 0x10);
    mEnvelope.SetEnvelope(value & 0x0F);
}

void Pulse::WriteReg1(byte value)
{
    mSweepEnabled = value & 0x80;
    mSweepPeriod = (value >> 4) & 0x07;
    mSweepNegate = value & 0x08;
    mSweepShift = value & 0x07;
    mSweepReload = true;
}

void Pulse::WriteReg2(byte value)
{
    mTimerReg = Word(mTimerReg >> 8, value);
    mTimerMuted = (mTimerReg < 8);
}

void Pulse::WriteReg3(byte value)
{
    mTimerReg = Word((value & 0x07), mTimerReg & 0xFF);
    mSequencePos = 0;
    mTimerMuted = (mTimerReg < 8);
    mLengthCounter.SetLength(value >> 3);
    mEnvelope.SetStartFlag();
}

void Pulse::QuarterFrame()
{
    mEnvelope.Tick();
}

void Pulse::HalfFrame()
{
    mLengthCounter.Tick();

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
    if (mSweepMuted || mTimerMuted || mLengthCounter.Silenced())
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
            auto sample = mSequenceTable[mDuty][mSequencePos] ? mEnvelope.Volume() : 0;
            mSynth.update(time, sample);
        }
    }
}






///////////////////////////////////////////////////////////////////////////
// TRIANGLE
////////////////////////////////////////////////////


Triangle::Triangle(Blip_Buffer* buffer)
{
    mSynth.volume(0.12765 * 0.5);
    mSynth.output(buffer);

    RegisterField("length", &mLengthCounter);
    RegisterField("timer", &mTimer);
    RegisterField("timerreg", &mTimerReg);
    RegisterField("control", &mControl);
    RegisterField("reload", &mCounterReload);
    RegisterField("counter", &mCounter);
    RegisterField("counterreg", &mCounterReg);
    RegisterField("sequence", &mSequencePos);
}

void Triangle::SetEnabled(bool enabled)
{
    mLengthCounter.SetEnabled(enabled);
}

void Triangle::WriteReg0(byte value)
{
    mControl = (value & 0x80);
    mCounterReg = (value & 0x7F);
    mLengthCounter.SetHalted(value & 0x80);
}

void Triangle::WriteReg2(byte value)
{
    mTimerReg = Word(mTimerReg >> 8, value);
}

void Triangle::WriteReg3(byte value)
{
    mTimerReg = Word((value & 0x07), mTimerReg & 0xFF);
    mSequencePos = 0;
    mCounterReload = true;
    mLengthCounter.SetLength(value >> 3);
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
    mLengthCounter.Tick();
}

void Triangle::Process(word cycles)
{
    if (!mCounter || mLengthCounter.Silenced())
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



///////////////////////////////////////////////////////////////////////////
// NOISE
////////////////////////////////////////////////////


Noise::Noise(Blip_Buffer* buffer)
{
    mSynth.volume(0.0741 * 0.5);
    mSynth.output(buffer);

    RegisterField("length", &mLengthCounter);
    RegisterField("envelope", &mEnvelope);
    RegisterField("timer", &mTimer);
    RegisterField("timerreg", &mTimerReg);
    RegisterField("mode", &mShiftMode);
    RegisterField("shift", &mShift);
    RegisterField("last", &mLast);
}

void Noise::SetEnabled(bool enabled)
{
    mLengthCounter.SetEnabled(enabled);
}

void Noise::WriteReg0(byte value)
{
    mLengthCounter.SetHalted(value & 0x20);
    mEnvelope.SetLoopFlag(value & 0x20);
    mEnvelope.SetConstantVolume(value & 0x10);
    mEnvelope.SetEnvelope(value & 0x0F);
}

void Noise::WriteReg2(byte value)
{
    mShiftMode = (value & 0x80) ? 8 : 13;
    mTimerReg = mPeriodTable[(value & 0x0F)];
}

void Noise::WriteReg3(byte value)
{
    mLengthCounter.SetLength(value >> 3);
    mEnvelope.SetStartFlag();
}

void Noise::QuarterFrame()
{
    mEnvelope.Tick();
}

void Noise::HalfFrame()
{
    mLengthCounter.Tick();
}

void Noise::Process(word cycles)
{
    if (mLengthCounter.Silenced())
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
            auto sample = (mShift & 1) ? mEnvelope.Volume() : 0;
            //std::cout << "sample " << (int)mTimer << " " << (int)sample << " " << (int)Volume() << " " << (int)mShift << "\n";
            if (sample != mLast) {
                mSynth.update(time, sample);
                mLast = sample;
            }

            mShift = feedback | (mShift >> 1);
        }
    }
}



///////////////////////////////////////////////////////////////////////////
// DMC
////////////////////////////////////////////////////


DMC::DMC(Bus* bus, Blip_Buffer* buffer) : mBus(bus)
{
    mSynth.volume(0.42545 * 0.5);
    mSynth.output(buffer);

    RegisterField("timer", &mTimer);
    RegisterField("timerreg", &mTimerReg);
    RegisterField("loop", &mLoop);
    RegisterField("sample", &mSampleBuffer);
    RegisterField("remaining", &mSamplesRemaining);
    RegisterField("address", &mSampleAddress);
    RegisterField("curremaining", &mCurrentRemaining);
    RegisterField("curaddress", &mCurrentAddress);
    RegisterField("silenced", &mSilenced);
    RegisterField("count", &mShiftCount);
    RegisterField("output", &mOutput);
    RegisterField("shift", &mShiftReg);
}

void DMC::WriteReg0(byte value)
{
//    mDMC.SetIRQ(value & 0x80);
    mLoop = (value & 0x40);
    mTimerReg = mPeriodTable[(value & 0x0F)];
}

void DMC::WriteReg1(byte value)
{
    mOutput = value;
}

void DMC::WriteReg2(byte value)
{
    mSampleAddress = 0xC000 | (value << 6);
}

void DMC::WriteReg3(byte value)
{
    mSamplesRemaining = (value << 4) | 1;
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




///////////////////////////////////////////////////////////////////////////
// APU
////////////////////////////////////////////////////



APU::APU(Bus* bus) : mBus(bus), mPulse0(&mBuffer, 0), mPulse1(&mBuffer, 1), mTriangle(&mBuffer), mNoise(&mBuffer), mDMC(bus, &mBuffer)
{
    RegisterField("pulse0", &mPulse0);
    RegisterField("pulse1", &mPulse1);
    RegisterField("triangle", &mTriangle);
    RegisterField("noise", &mNoise);
    RegisterField("dmc", &mDMC);
    RegisterField("framereg", &mFrameReg);
    RegisterField("frame", &mFrameCount);
    RegisterField("cycle", &mCycle);
    RegisterField("processed", &mProcessed);
    RegisterField("last", &mLastProcessed);
    RegisterField("quarter", &mQuarterFrame);
    RegisterField("half", &mHalfFrame);
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
    case 0x00: mPulse0.WriteReg0(value); break;
    case 0x01: mPulse0.WriteReg1(value); break;
    case 0x02: mPulse0.WriteReg2(value); break;
    case 0x03: mPulse0.WriteReg3(value); break;
    // PULSE 1 REGISTERS
    case 0x04: mPulse1.WriteReg0(value); break;
    case 0x05: mPulse1.WriteReg1(value); break;
    case 0x06: mPulse1.WriteReg2(value); break;
    case 0x07: mPulse1.WriteReg3(value); break;
    // TRIANGLE REGISTERS
    case 0x08: mTriangle.WriteReg0(value); break;
    case 0x0A: mTriangle.WriteReg2(value); break;
    case 0x0B: mTriangle.WriteReg3(value); break;
    // TRIANGLE REGISTERS
    case 0x0C: mNoise.WriteReg0(value); break;
    case 0x0E: mNoise.WriteReg2(value); break;
    case 0x0F: mNoise.WriteReg3(value); break;
    // DMC REGISTERS
    case 0x10: mDMC.WriteReg0(value); break;
    case 0x11: mDMC.WriteReg1(value); break;
    case 0x12: mDMC.WriteReg2(value); break;
    case 0x13: mDMC.WriteReg3(value); break;

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



