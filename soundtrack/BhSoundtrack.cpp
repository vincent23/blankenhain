#include "BhSoundtrack.h"

#include "BlankenhainPlayer.h"

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
#include "WidthEffect.h"
#include "gmsynthInstrument.h"
#include "DistortionEffect.h"
#include "DelayEffect.h"
#include "freeverbEffect.h"
#include "VolumeEffect.h"
#include "EqualizerEffect.h"
#include "basedrum1Instrument.h"
#include "CompressorEffect.h"
#include "FilterEffect.h"
#include "ChorusEffect.h"
#include "FmInstrument.h"
#include "RhythmgateEffect.h"
#include "glidePolyblepInstrument.h"
#include "LimiterEffect.h"

void blankenhain::render(float* buffer)
{
#include "Input.inl"
	BlankenhainPlayer player;
	player.play(song, buffer);
}

unsigned int blankenhain::lengthInSamples()
{
	return ((LENGTH_IN_SAMPLES - 1) / constants::blockSize + 1) * constants::blockSize;
}