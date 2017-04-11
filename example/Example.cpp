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

int main(int argc, char* argv[]) {
#include "Input.inl"
	BlankenhainPlayer player;
	player.play(song);
	return 0;
}