#pragma once

#include "common.h"
#include "interface.h"
#include "blipbuffer.h"
#include "serializer.h"


class Bus;


class LengthCounter : public Serializable
{
public:
    LengthCounter();

public:
    void Tick();
    bool Silenced();

public:
    void SetEnabled(bool enabled);
    void SetHalted(bool halted);
    void SetLength(byte length);


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


class Envelope : public Serializable
{
public:
    Envelope();

public:
    void Tick();
    byte Volume();

public:
    void SetEnvelope(byte value);
    void SetConstantVolume(bool constant);
    void SetLoopFlag(bool loop);
    void SetStartFlag();


private:
    bool mLoopFlag = false;
    bool mStartFlag = false;
    bool mConstantVolume = false;
    byte mDecayLevel = 0;
    byte mDivider = 0;
    byte mVolume = 0;
};


//class Pulse : public LengthCounter, public Envelope, public Serializable
class Pulse : public Serializable
{
public:
    Pulse(Blip_Buffer* buffer, byte channel);

public:
    void SetEnabled(bool enabled);
    void WriteReg0(byte value);
    void WriteReg1(byte value);
    void WriteReg2(byte value);
    void WriteReg3(byte value);
    void QuarterFrame();
    void HalfFrame();
    void Process(word cycles);

private:
    // SYNTH
    Blip_Synth<blip_good_quality, 15> mSynth;

    // LENGTH COUNTER
    LengthCounter mLengthCounter;

    // ENVELOPE
    Envelope mEnvelope;

    // TIMER
    word mTimer = 0;
    word mTimerReg = 0;
    bool mTimerMuted = true;

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


class Triangle : public Serializable
{
public:
    Triangle(Blip_Buffer* buffer);

public:
    void SetEnabled(bool enabled);
    void WriteReg0(byte value);
    void WriteReg2(byte value);
    void WriteReg3(byte value);
    void QuarterFrame();
    void HalfFrame();
    void Process(word cycles);

private:
    // SYNTH
    Blip_Synth<blip_good_quality, 15> mSynth;

    // LENGTH COUNTER
    LengthCounter mLengthCounter;

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



class Noise: public Serializable
{
public:
    Noise(Blip_Buffer* buffer);

public:
    void SetEnabled(bool enabled);
    void WriteReg0(byte value);
    void WriteReg2(byte value);
    void WriteReg3(byte value);
    void QuarterFrame();
    void HalfFrame();
    void Process(word cycles);

private:
    // SYNTH
    Blip_Synth<blip_med_quality, 15> mSynth;

    // LENGTH COUNTER
    LengthCounter mLengthCounter;

    // ENVELOPE
    Envelope mEnvelope;

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


class DMC : public Serializable
{
public:
    DMC(Bus* bus, Blip_Buffer* buffer);

public:
    void WriteReg0(byte value);
    void WriteReg1(byte value);
    void WriteReg2(byte value);
    void WriteReg3(byte value);
    void Process(word cycles);

public:
    void SetEnabled(bool enabled);
    void SetStart(bool start);
    void SetIRQ(bool irq);
    void SetLoop(bool loop);
    void SetRate(byte rate);
    void SetSample(byte sample);
    void SetAddress(byte address);
    void SetLength(byte length);

private:
    // DMC
    Bus* mBus = nullptr;
    bool mEnabled = false;

    // SYNTH
    Blip_Synth<blip_med_quality, 127> mSynth;

    // MEMORY READER
    bool mLoop = false;
    byte mSampleBuffer = 0;
    word mSamplesRemaining = 0;
    word mSampleAddress = 0;
    word mCurrentRemaining = 0;
    word mCurrentAddress = 0;

    // OUTPUT UNIT
    bool mSilenced = false;
    byte mShiftCount = 0;
    byte mOutput = 0;
    byte mShiftReg = 0;

    // TIMER
    word mTimer = 0;
    word mTimerReg = 428;
    word mPeriodTable[16] = {
        428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106, 84, 72, 54
    };
};


class APU : public Serializable
{

    struct framecount : reg // write only
    {
        byte m = 0;    // sequencer mode - 0: 4 step; 1: 5 step
        byte i = 0;    // interrupt inhibit
        void operator =(byte val) { m = (val >> 7) & 1; i = (val >> 6) & 1; }
        operator byte() const { return (m << 7) | (i << 6); }
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
    DMC mDMC;


private:
    framecount mFrameReg;
    word mFrameCount = 0;
    uint64_t mCycle = 0;
    uint64_t mProcessed = 0;
    uint64_t mLastProcessed = 0;
    uint64_t mQuarterFrame = 7457;
    uint64_t mHalfFrame = 14914;
};


