#pragma once

class SongInfo;
class GroupDevice;

class Song
{
public:
	Song(SongInfo& songInfo, GroupDevice& master);

private:
	SongInfo& songInfo;
	GroupDevice& master;
};