#pragma once

class MidiTrack;
class Sample;

class SongInfo
{
public:
	SongInfo(MidiTrack** midiTracks, Sample* sendBuffers, unsigned int numberOfReturnTracks, float bpm, unsigned int lengthInSamples);

	MidiTrack** midiTracks;
	Sample* sendBuffers;
	unsigned int numberOfReturnTracks;
	float bpm;
	unsigned int lengthInSamples;
};