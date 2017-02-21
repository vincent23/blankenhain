#pragma once

class SongInfo;
class ChainDevice;

class Song
{
public:
	Song(SongInfo& songInfo, ChainDevice& master);

private:
	SongInfo& songInfo;
	ChainDevice& master;
};