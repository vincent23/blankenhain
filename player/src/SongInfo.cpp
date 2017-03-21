#include "SongInfo.h"

SongInfo::SongInfo(MidiTrack** midiTracks_, float bpm_, unsigned int lengthInSamples_)
	: midiTracks(midiTracks_)
	, bpm(bpm_)
	, lengthInSamples(lengthInSamples_)
{}