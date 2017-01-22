#include "Song.h"

Song::Song(MidiTrack* tracks_, GroupDevice & master_)
	: tracks(tracks_)
	, master(master_)
{}
