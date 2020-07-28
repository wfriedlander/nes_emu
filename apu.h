#pragma once

#include "common.h"
#include "bus.h"
#include "interface.h"
#include "blipbuffer.h"



class LengthCounter
{
public:
    void SetEnabled(bool enabled);
    void SetHalted(bool halted);
    void SetLength(byte length);

protected:
    void TickLengthCounter();
    bool Silenced();

private:
    bool mEnabled = false;
    bool mHalted = false;
    byte mLength = 0;

private:
    byte mLengthTable[32] = {
        10,  254,   20,    2,
        40,    4,   80,    6,
        160,   8,   60,   10,
        14,   12,   26,   14,
        12,   16,   24,   18,
        48,   20,   96,   22,
        192,  24,   72,   26,
        16,   28,   32,   30
    };
};


class Envelope
{
public:
    void SetEnvelope(byte value);
    void SetConstantVolume(bool constant);
    void SetLoopFlag(bool loop);
    void SetStartFlag();

protected:
    void TickEnvelope();
    byte Volume();

private:
    bool mLoopFlag = false;
    bool mStartFlag = false;
    bool mConstantVolume = false;
    byte mDecayLevel = 0;
    byte mDivider = 0;
    byte mVolume = 0;
};


class Pulse : public LengthCounter, public Envelope
{
public:
    Pulse(Blip_Buffer* buffer, byte channel);

public:
    void QuarterFrame();
    void HalfFrame();
    void Process(word cycles);

public:
    void SetDuty(byte duty);
    void SetSweep(byte sweep);
    void SetTimerHigh(byte period);
    void SetTimerLow(byte period);

private:
    // SYNTH
    Blip_Synth<blip_good_quality, 15> mSynth;

    // TIMER
    word mTimer = 0;
    word mTimerReg = 0;

    // SWEEP
    bool mSweepReload = false;
    bool mSweepEnabled = false;
    bool mSweepNegate = false;
    bool mSweepMuted = false;
    byte mSweep = 0;
    byte mSweepPeriod = 0;
    byte mSweepShift = 0;
    byte mChannel = 0;

    // SEQUENCER
    byte mDuty = 0;
    byte mSequencePos = 0;
    byte mSequenceTable[4][8] = {
        {0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 0, 0, 0},
        {1, 0, 0, 1, 1, 1, 1, 1}
    };
};


class Triangle : public LengthCounter
{
public:
    Triangle(Blip_Buffer* buffer);

public:
    void QuarterFrame();
    void HalfFrame();
    void Process(word cycles);

public:
    void SetControlFlag(bool flag);
    void SetCounter(byte value);
    void SetTimerHigh(byte period);
    void SetTimerLow(byte period);
    void SetCounterReloadFlag();

private:
    // SYNTH
    Blip_Synth<blip_good_quality, 15> mSynth;

    // TIMER
    word mTimer = 0;
    word mTimerReg = 0;

    // COUNTER
    bool mControl = false;
    bool mCounterReload = false;
    word mCounter = 0;
    word mCounterReg = 0;

    // SEQUENCER
    byte mSequencePos = 0;
    byte mSequenceTable[32] = {
        15, 14, 13, 12, 11, 10, 9,  8,
        7,  6,  5,  4,  3,  2,  1,  0,
        0,  1,  2,  3,  4,  5,  6,  7,
        8,  9,  10, 11, 12, 13, 14, 15
    };
};



class Noise: public LengthCounter, public Envelope
{
public:
    Noise(Blip_Buffer* buffer);

public:
    void QuarterFrame();
    void HalfFrame();
    void Process(word cycles);

public:
    void SetMode(bool mode);
    void SetPeriod(byte period);

private:
    // SYNTH
    Blip_Synth<blip_med_quality, 15> mSynth;

    // SHIFT
    byte mShiftMode = 13;
    word mShift = 1;
    byte mLast = 0;

    // TIMER
    word mTimer = 0;
    word mTimerReg = 0;
    word mPeriodTable[16] = {
        4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
    };
};




class APU
{
    struct pulse // write only
    {
        byte d = 0;    // duty cycle
        byte l = 0;    // length counter halt
        byte c = 0;    // constant volume
        byte v = 0;    // volume/envelope divider period
        void operator =(byte val) { d = (val >> 6) & 3; l = (val >> 5) & 1; c = (val >> 4) & 1; l = (val >> 0) & 15; }
    };

    struct pulsesweep // write only
    {
        byte e = 0;    //
        byte p = 0;    //
        byte n = 0;    //
        byte s = 0;    //
        void operator =(byte val) { e = (val >> 7) & 1; p = (val >> 4) & 7; n = (val >> 3) & 1; s = (val >> 0) & 7; }
    };

    struct framecount // write only
    {
        byte m = 0;    // sequencer mode - 0: 4 step; 1: 5 step
        byte i = 0;    // interrupt inhibit
        void operator =(byte val) { m = (val >> 7) & 1; i = (val >> 6) & 1; }
    };


public:
    APU(Bus* bus);
    void SetAudioBackend(IAudio* audio);
    void EnableLogging(bool enable);

public:
    void Execute(word cycles);
    void FrameDone();
    void Reset();

public:
    byte RegisterRead(word address);
    void RegisterWrite(word address, byte value);

private:
    void QuarterFrame();
    void HalfFrame();
    void ProcessSamples(word cycles);

private:
    Bus* mBus = nullptr;
    bool mLogging = false;
    Blip_Buffer mBuffer;

private:
    Pulse mPulse0;
    Pulse mPulse1;
    Triangle mTriangle;
    Noise mNoise;


private:
    framecount mFrameReg;
    word mFrameCount = 0;
    uint64_t mCycle = 0;
    uint64_t mProcessed = 0;
    uint64_t mQuarterFrame = 7457;
    uint64_t mHalfFrame = 14914;
};


