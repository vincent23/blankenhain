#pragma once

#include "BhSoundtrack.h"

#include "BlankenhainPlayer.h"

#include "Options.h"

#include "FpuState.h"

#include "MidiTrack.h"
#include "ParameterTrack.h"
#include "InstrumentDevice.h"
#include "ChainDevice.h"
#include "GroupDevice.h"
#include "SongInfo.h"
#include "Song.h"
#include "ReturnDevice.h"
#include "SendDevice.h"

#include "polyblepInstrument.h"
#include "PanEffect.h"
#include "gmsynthInstrument.h"
#include "DistortionEffect.h"
#include "DelayEffect.h"
#include "ReverbEffect.h"
#include "VolumeEffect.h"
#include "EqualizerEffect.h"
#include "CompressorEffect.h"
#include "FilterEffect.h"
#include "ChorusEffect.h"
#include "FmInstrument.h"
#include "RhythmgateEffect.h"
#include "MonosynthInstrument.h"
#include "LimiterEffect.h"

#include "ExampleInstrument.h"
#include "AbletonTrackMixerVolumeEffect.h"

void blankenhain::render(float* buffer)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	try
	{
		std::cout << "Warnings and Checks are enabled. Blankenhain will exit upon error." << std::endl;
#endif

	//////////////////////////////////////
	// Set up FPU 
	//////////////////////////////////////
	FpuState fpuState;
	//////////////////////////////////////
#include "Input.inl"

	BlankenhainPlayer player;
	player.play(song, buffer);

#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS

	}
	catch (const std::exception& exp)
	{
		std::cout << exp.what() << std::endl;
	}
#endif
}

unsigned int blankenhain::lengthInSamples()
{
	return ((LENGTH_IN_SAMPLES - 1) / constants::blockSize + 1) * constants::blockSize;
}