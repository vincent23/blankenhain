#include "Song.h"

Song::Song(SongInfo& songInfo_, ChainDevice& master_)
	: songInfo(songInfo_)
	, master(master_)
{}