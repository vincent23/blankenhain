#pragma once

class MidiTrack;

class SongInfo
{
public:
	SongInfo(MidiTrack* midiTracks, float bpm);

	MidiTrack* midiTracks;
	float bpm;
};