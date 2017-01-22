#pragma once

class MidiTrack;
class GroupDevice;

class Song
{
public:
	Song(MidiTrack* tracks, GroupDevice& master);

private:
	MidiTrack* tracks;
	GroupDevice& master;
};