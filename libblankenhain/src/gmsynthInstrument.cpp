#include "gmsynthInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"
#include "gmSound.h"
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

gmsynthInstrument::gmsynthInstrument()
	: InstrumentBase(10, 8)
{
	ParameterBundle* params = getPointerToParameterBundle();


	params->getParameter(0) = new FloatParameter(0.06f, NormalizedRange(0.05f, 1700.f, 0.3f), "attack", "ms");
	params->getParameter(1) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "hold", "ms");
	params->getParameter(2) = new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio");
	params->getParameter(3) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "decay", "ms");
	params->getParameter(4) = new BoolParameter(false, "sustainBool");
	params->getParameter(5) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "sustain", "ms");
	params->getParameter(6) = new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio");
	params->getParameter(7) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "release", "ms");
	params->getParameter(8) = new BoolParameter(false, "loop");
	//params->getParameter(9) = new FloatParameter(0.f, NormalizedRange(0, 235), "instrument", "");
	params->getParameter(9) = new DiscreteParameter(235, "instrument");

	for (unsigned int i = 0; i < 128; i++) 
	{
		interpolatedSounds[i] = nullptr;
	}
	// load instrument 0
	{
		gmSoundRegion* tempRegions;
		gmSoundRegion tempRegion;
		tempRegions = new gmSoundRegion[10];
		tempRegion = gmSoundRegion();
		tempRegion.lowest = 0;
		tempRegion.highest = 38;
		tempRegion.rootNote = 36;
		tempRegion.loopStart = 11989;
		tempRegion.loopLength = 1682;
		tempRegion.isLoopable = true;
		tempRegion.sampleLength = 27344;
		tempRegion.startByte = 1869034;
		tempRegions[0] = tempRegion;
		tempRegion = gmSoundRegion();
		tempRegion.lowest = 39;
		tempRegion.highest = 43;
		tempRegion.rootNote = 41;
		tempRegion.loopStart = 10830;
		tempRegion.loopLength = 2272;
		tempRegion.isLoopable = true;
		tempRegion.sampleLength = 26206;
		tempRegion.startByte = 1896544;
		tempRegions[1] = tempRegion;
		tempRegion = gmSoundRegion();
		tempRegion.lowest = 44;
		tempRegion.highest = 50;
		tempRegion.rootNote = 48;
		tempRegion.loopStart = 9313;
		tempRegion.loopLength = 1854;
		tempRegion.isLoopable = true;
		tempRegion.sampleLength = 22336;
		tempRegion.startByte = 1922916;
		tempRegions[2] = tempRegion;
		tempRegion = gmSoundRegion();
		tempRegion.lowest = 51;
		tempRegion.highest = 59;
		tempRegion.rootNote = 56;
		tempRegion.loopStart = 8309;
		tempRegion.loopLength = 1487;
		tempRegion.isLoopable = true;
		tempRegion.sampleLength = 19594;
		tempRegion.startByte = 1945418;
		tempRegions[3] = tempRegion;
		tempRegion = gmSoundRegion();
		tempRegion.lowest = 60;
		tempRegion.highest = 67;
		tempRegion.rootNote = 64;
		tempRegion.loopStart = 9176;
		tempRegion.loopLength = 1339;
		tempRegion.isLoopable = true;
		tempRegion.sampleLength = 21032;
		tempRegion.startByte = 1965178;
		tempRegions[4] = tempRegion;
		tempRegion = gmSoundRegion();
		tempRegion.lowest = 68;
		tempRegion.highest = 74;
		tempRegion.rootNote = 73;
		tempRegion.loopStart = 6194;
		tempRegion.loopLength = 1393;
		tempRegion.isLoopable = true;
		tempRegion.sampleLength = 15176;
		tempRegion.startByte = 1986376;
		tempRegions[5] = tempRegion;
		tempRegion = gmSoundRegion();
		tempRegion.lowest = 75;
		tempRegion.highest = 81;
		tempRegion.rootNote = 78;
		tempRegion.loopStart = 6447;
		tempRegion.loopLength = 1221;
		tempRegion.isLoopable = true;
		tempRegion.sampleLength = 15338;
		tempRegion.startByte = 2001718;
		tempRegions[6] = tempRegion;
		tempRegion = gmSoundRegion();
		tempRegion.lowest = 82;
		tempRegion.highest = 88;
		tempRegion.rootNote = 84;
		tempRegion.loopStart = 5976;
		tempRegion.loopLength = 1368;
		tempRegion.isLoopable = true;
		tempRegion.sampleLength = 14690;
		tempRegion.startByte = 2017222;
		tempRegions[7] = tempRegion;
		tempRegion = gmSoundRegion();
		tempRegion.lowest = 89;
		tempRegion.highest = 97;
		tempRegion.rootNote = 92;
		tempRegion.loopStart = 5392;
		tempRegion.loopLength = 1049;
		tempRegion.isLoopable = true;
		tempRegion.sampleLength = 12884;
		tempRegion.startByte = 2032078;
		tempRegions[8] = tempRegion;
		tempRegion = gmSoundRegion();
		tempRegion.lowest = 98;
		tempRegion.highest = 127;
		tempRegion.rootNote = 103;
		tempRegion.loopStart = 4686;
		tempRegion.loopLength = 155;
		tempRegion.isLoopable = true;
		tempRegion.sampleLength = 9684;
		tempRegion.startByte = 2045128;
		tempRegions[9] = tempRegion;
		gmInstrument instrument("", 10, tempRegions);
		delete[] tempRegions;
		loadMidiInstrument(instrument);
	}
}

gmsynthInstrument::~gmsynthInstrument()
{
	for (unsigned int i = 0; i < 128; i++) {
		if (interpolatedSounds[i] != nullptr) {
			delete interpolatedSounds[i];
		}
	}
}

void gmsynthInstrument::processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples)
{

	float attack = getInterpolatedParameter(0).get();
	float hold = getInterpolatedParameter(1).get();
	float holdLevel = getInterpolatedParameter(2).get();
	float decay = getInterpolatedParameter(3).get();
	bool sustainOn = getInterpolatedParameter(4).get() > 0.5 ? true : false;
	float sustainLevel = getInterpolatedParameter(6).get();
	float sustain = getInterpolatedParameter(5).get();
	float release = getInterpolatedParameter(7).get();
	bool loop = getInterpolatedParameter(8).get() > 0.5f;

	gmSound* sound = interpolatedSounds[voice.key];
	if (sound == nullptr) {
		for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++) {
			buffer[sampleIndex] = Sample(0);
		}
	}
	else {
		unsigned int sampleSize = sound->interpolatedBufferSize;
		loop = loop && sound->isLoopable;
		for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++) {
			unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;

			if (loop && deltaT > sound->loopStart) {
				deltaT -= sound->loopStart;
				deltaT %= sound->loopLength;
				deltaT += sound->loopStart;
			}
			if (deltaT < sampleSize) {
				buffer[sampleIndex] = Sample(sound->interpolatedBuffer[deltaT]);
			}
			else {
				buffer[sampleIndex] = Sample(0);
			}

			performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
		}
	}
}

void gmsynthInstrument::loadMidiInstrument(const gmInstrument& instrument)
{
	char paths[] = "\\drivers\x0"
		"\\drivers\\etc";
	char fileName[] = "\\gm.dls";
	char path[256];
	char *pathsPtr = paths;

	HANDLE h;
	do {
		int pos = GetSystemDirectory(path, 256);
		while (*pathsPtr) path[pos++] = *pathsPtr++; pathsPtr++;
		for (unsigned int i = 0; i < sizeof(fileName); i++) path[pos++] = fileName[i];
		h = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	} while (h == INVALID_HANDLE_VALUE);

	for (unsigned int i = 0u; i < 128; i++)
	{
		if (interpolatedSounds[i] != nullptr) {
			delete interpolatedSounds[i];
		}

		gmSoundRegion* region = instrument.findTargetRegion(i);
		if (region != nullptr) {
			interpolatedSounds[i] = new gmSound(*region, h, i);
		}
		else {
			interpolatedSounds[i] = nullptr;
		}
	}

	CloseHandle(h);
}