#include "SongInfo.h"

SongInfo::SongInfo(MidiTrack** midiTracks_, Sample* sendBuffers_, unsigned int numberOfReturnTracks_, float bpm_, unsigned int lengthInSamples_)
	: midiTracks(midiTracks_)
	, sendBuffers(sendBuffers_)
	, numberOfReturnTracks(numberOfReturnTracks_)
	, bpm(bpm_)
	, lengthInSamples(lengthInSamples_)
{}