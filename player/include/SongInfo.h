#pragma once

class MidiTrack;

class SongInfo
{
public:
	SongInfo(MidiTrack* midiTracks, float bpm, unsigned int lengthInSamples);

	MidiTrack* midiTracks;
	float bpm;
	unsigned int lengthInSamples;
};