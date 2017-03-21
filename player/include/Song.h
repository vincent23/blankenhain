#pragma once

class SongInfo;
class ChainDevice;

class Song
{
public:
	Song(SongInfo& songInfo, ChainDevice& master);

	SongInfo& songInfo;
	ChainDevice& master;
};