#pragma once

#include "common.h"
#include "bus.h"
#include "interface.h"



class LengthCounter
{
public:
	void SetEnabled(bool enabled);
	void SetHalted(bool halted);
	void SetLength(byte length);
	void HalfStep();
	byte Count();


private:
	bool mEnabled = false;
	bool mHalted = false;
	byte mCount = 0;
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

class Noise
{
public:
	void RegisterWrite(word address, byte value);
	const std::vector<byte>& ProcessSamples(word cycles);
	void QuarterStep();
	void HalfStep();
	void Enable(bool enabled);

private:
	bool mStart = true;
	bool mConstantVolume = false;
	bool mLoop = false;
	byte mDecay = 0;
	byte mDivider = 0;
	byte mEnvelope = 0;
	byte mShiftMode = 0;
	word mTimer = 0;
	word mTimerReg = 0;
	word mShift = 0;
	LengthCounter mLengthCounter;

private:
	std::vector<byte> mSamples;

private:
	word mPeriodTable[16] = {
		4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
	};
};

class Triangle
{
public:
	void RegisterWrite(word address, byte value);
	const std::vector<byte>& ProcessSamples(word cycles);
	void QuarterStep();
	void HalfStep();
	void Enable(bool enabled);

private:
	bool mControl = false;
	bool mLinearReload = false;
	word mTimer = 0;
	word mTimerReg = 0;
	word mLinearCounter = 0;
	word mLinearCounterReg = 0;
	byte mSequencePos = 0;
	LengthCounter mLengthCounter;

private:
	std::vector<byte> mSamples;

private:
	byte mSequenceTable[32] = {
		15, 14, 13, 12, 11, 10, 9,  8,
		7,  6,  5,  4,  3,  2,  1,  0,
		0,  1,  2,  3,  4,  5,  6,  7,
		8,  9,  10, 11, 12, 13, 14, 15
	};
};


class Pulse
{
public:
	Pulse(byte channel);

public:
	void RegisterWrite(word address, byte value);
	const std::vector<byte>& ProcessSamples(word cycles);
	void QuarterStep();
	void HalfStep();
	void Enable(bool enabled);

private:
	// ENVELOPE
	bool mStart = true;
	bool mConstantVolume = false;
	bool mLoop = false;
	byte mDecay = 0;
	byte mDivider = 0;
	byte mEnvelope = 0;

private:
	// TIMER
	word mTimer = 0;
	word mTimerReg = 0;

private:
	// SEQUENCER
	byte mDuty = 0;
	byte mSequencePos = 0;

private:
	// LENGTH COUNTER
	bool mEnabled = false;
	byte mLength = 0;

private:
	// SWEEP
	bool mSweepReload = false;
	bool mSweepEnabled = false;
	bool mSweepNeg = false;
	bool mMuted = false;
	byte mSweep = 0;
	byte mSweepPeriod = 0;
	byte mSweepShift = 0;
	byte mChannel = 0;

private:
	std::vector<byte> mSamples;

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

	byte mSequenceTable[4][8] = {
		{0, 1, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 0, 0, 0, 0, 0},
		{0, 1, 1, 1, 1, 0, 0, 0},
		{1, 0, 0, 1, 1, 1, 1, 1}
	};
};


class APU
{
    struct pulse : reg // write only
	{
		byte d = 0;    // duty cycle
		byte l = 0;    // length counter halt
		byte c = 0;    // constant volume
		byte v = 0;    // volume/envelope divider period
        void operator =(byte val) { d = (val >> 6) & 3; l = (val >> 5) & 1; c = (val >> 4) & 1; v = (val >> 0) & 15; }
        operator byte() const { return (d << 6 | l << 5 | c << 4 |  v); };
	};

    struct pulsesweep : reg // write only
	{
		byte e = 0;    // 
		byte p = 0;    // 
		byte n = 0;    // 
		byte s = 0;    // 
		void operator =(byte val) { e = (val >> 7) & 1; p = (val >> 4) & 7; n = (val >> 3) & 1; s = (val >> 0) & 7; }
        operator byte() const { return (e << 7 | p << 4 | n << 3 |  s); };
	};

    struct framecount : reg // write only
	{
		byte m = 0;    // sequencer mode - 0: 4 step; 1: 5 step
		byte i = 0;    // interrupt inhibit
		void operator =(byte val) { m = (val >> 7) & 1; i = (val >> 6) & 1; }
        operator byte() const { return (m << 7 | i << 6); };
	};


public:
	APU(Bus* bus);
    void SetAudioBackend(IAudio* audio);

public:
	void Execute(word cycles);
	void FrameDone();
	void Reset();

public:
	byte RegisterRead(word address);
	void RegisterWrite(word address, byte value);

private:
	void AdvanceFrameSequence();
	void QuarterClock();
	void HalfClock();
	void ProcessSamples();

private:
	Bus* mBus = nullptr;
    IAudio* mAudio = nullptr;

private:
	Pulse mPulse0;
	Pulse mPulse1;
	Triangle mTriangle;
	Noise mNoise;

private:
	framecount mFrameReg;
	AudioBuffer mSamples;

private:
	word mPendingCycles = 0;
	word mFrameCount = 0;
	byte mFramePosition = 0;
	int mActualFrame = 0;

private:
	word mFrameTimings[5] = {
		7457,
		14913,
		22371,
		29829,
		37281
	};

	std::vector<float> mPulseTable = {
        0.0f,
        0.011609139523578026f,
        0.022939481268011527f,
        0.03400094921689606f,
        0.04480300187617261f,
        0.05535465924895688f,
        0.06566452795600367f,
        0.07574082464884459f,
        0.08559139784946236f,
        0.09522374833850243f,
        0.10464504820333041f,
        0.11386215864759427f,
        0.12288164665523155f,
        0.13170980059397538f,
        0.14035264483627205f,
        0.1488159534690486f,
        0.15710526315789472f,
        0.16522588522588522f,
        0.1731829170024174f,
        0.18098125249301955f,
        0.18862559241706162f,
        0.19612045365662886f,
        0.20347017815646784f,
        0.21067894131185272f,
        0.21775075987841944f,
        0.2246894994354535f,
        0.2314988814317673f,
        0.23818248984115256f,
        0.2447437774524158f,
        0.2511860718171926f,
        0.25751258087706685f,
        0.2637263982899893f
	};

	std::vector<float> mTndTable = {
        0.0f,
        0.006699823979696262f,
        0.01334502018019487f,
        0.01993625400950099f,
        0.026474180112418616f,
        0.032959442587297105f,
        0.03939267519756107f,
        0.04577450157816932f,
        0.05210553543714433f,
        0.05838638075230885f,
        0.06461763196336215f,
        0.07079987415942428f,
        0.07693368326217241f,
        0.08301962620468999f,
        0.08905826110614481f,
        0.09505013744240969f,
        0.10099579621273477f,
        0.10689577010257789f,
        0.11275058364269584f,
        0.11856075336459644f,
        0.12432678795244785f,
        0.1300491883915396f,
        0.13572844811338536f,
        0.1413650531375568f,
        0.1469594822103333f,
        0.15251220694025122f,
        0.15802369193063237f,
        0.16349439490917161f,
        0.16892476685465738f,
        0.1743152521209005f,
        0.1796662885579421f,
        0.18497830763060993f,
        0.19025173453449087f,
        0.19548698830938505f,
        0.20068448195030472f,
        0.20584462251608032f,
        0.2109678112356332f,
        0.2160544436119733f,
        0.2211049095239788f,
        0.22611959332601225f,
        0.2310988739454269f,
        0.23604312497801538f,
        0.24095271478145042f,
        0.24582800656676793f,
        0.25066935848793903f,
        0.25547712372957787f,
        0.2602516505928307f,
        0.26499328257948945f,
        0.26970235847437257f,
        0.27437921242601526f,
        0.27902417402570834f,
        0.28363756838492643f,
        0.2882197162111822f,
        0.292770933882345f,
        0.29729153351945914f,
        0.3017818230580978f,
        0.3062421063182866f,
        0.31067268307302937f,
        0.31507384911547015f,
        0.3194458963247213f,
        0.32378911273039f,
        0.3281037825758322f,
        0.3323901863801631f,
        0.33664860099905314f,
        0.3408792996843372f,
        0.34508255214246325f,
        0.349258624591807f,
        0.3534077798188791f,
        0.3575302772334479f,
        0.36162637292260397f,
        0.3656963197037888f,
        0.3697403671768112f,
        0.3737587617748739f,
        0.37775174681463214f,
        0.38171956254530554f,
        0.38566244619686446f,
        0.3895806320273106f,
        0.3934743513690717f,
        0.3973438326745308f,
        0.40118930156070615f,
        0.405010980853104f,
        0.4088090906287582f,
        0.41258384825847705f,
        0.4163354684483128f,
        0.42006416328027124f,
        0.4237701422522769f,
        0.42745361231741014f,
        0.4311147779224318f,
        0.4347538410456096f,
        0.43837100123386197f,
        0.4419664556392331f,
        0.44554039905471293f,
        0.44909302394941686f,
        0.4526245205031371f,
        0.45613507664027986f,
        0.4596248780632002f,
        0.4630941082849479f,
        0.4665429486614358f,
        0.46997157842304194f,
        0.47338017470565896f,
        0.4767689125811996f,
        0.48013796508757145f,
        0.48348750325813084f,
        0.48681769615062515f,
        0.49012871087563703f,
        0.493420712624537f,
        0.49669386469695664f,
        0.49994832852779125f,
        0.5031842637137408f,
        0.5064018280393993f,
        0.5096011775029012f,
        0.5127824663411329f,
        0.5159458470545188f,
        0.5190914704313901f,
        0.5222194855719443f,
        0.5253300399118033f,
        0.528423279245178f,
        0.5314993477476477f,
        0.5345583879985607f,
        0.5376005410030638f,
        0.5406259462137686f,
        0.5436347415520602f,
        0.5466270634290563f,
        0.5496030467662235f,
        0.5525628250156552f,
        0.5555065301800212f,
        0.5584342928321915f,
        0.5613462421345432f,
        0.5642425058579547f,
        0.5671232104004943f,
        0.5699884808058077f,
        0.5728384407812124f,
        0.5756732127155f,
        0.5784929176964575f,
        0.5812976755281083f,
        0.5840876047476803f,
        0.5868628226423054f,
        0.5896234452654553f,
        0.5923695874531196f,
        0.595101362839729f,
        0.5978188838738291f,
        0.6005222618335111f,
        0.6032116068415997f,
        0.6058870278806079f,
        0.6085486328074569f,
        0.6111965283679723f,
        0.6138308202111536f,
        0.6164516129032258f,
        0.6190590099414757f,
        0.6216531137678758f,
        0.6242340257825014f,
        0.6268018463567424f,
        0.6293566748463153f,
        0.6318986096040777f,
        0.6344277479926501f,
        0.6369441863968464f,
        0.6394480202359187f,
        0.6419393439756177f,
        0.6444182511400732f,
        0.6468848343234979f,
        0.6493391852017159f,
        0.6517813945435207f,
        0.6542115522218658f,
        0.6566297472248885f,
        0.659036067666773f,
        0.6614306007984521f,
        0.6638134330181533f,
        0.6661846498817908f,
        0.6685443361132047f,
        0.670892575614252f,
        0.6732294514747513f,
        0.6755550459822829f,
        0.6778694406318475f,
        0.6801727161353863f,
        0.6824649524311629f,
        0.684746228693012f,
        0.6870166233394548f,
        0.6892762140426848f,
        0.6915250777374256f,
        0.693763290629662f,
        0.6959909282052493f,
        0.6982080652383982f,
        0.7004147758000423f,
        0.7026111332660865f,
        0.70479721032554f,
        0.7069730789885358f,
        0.7091388105942369f,
        0.7112944758186339f,
        0.7134401446822323f,
        0.7155758865576349f,
        0.7177017701770176f,
        0.7198178636395035f,
        0.7219242344184336f,
        0.7240209493685391f,
        0.7261080747330146f,
        0.7281856761504939f,
        0.7302538186619317f,
        0.7323125667173908f,
        0.734361984182737f,
        0.7364021343462434f,
        0.7384330799251054f,
        0.7404548830718675f,
        0.742467605380763f
	};

};

