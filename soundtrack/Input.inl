
Device* deviceList[] = { nullptr };
ChainDevice device(0, deviceList);
MidiTrack* midiTracks[] = { nullptr };
Sample sendBuffers[constants::blockSize * 2];
SongInfo songInfo(midiTracks, sendBuffers, 2, 124.0, 8108710);
Song song(songInfo, device);
#define LENGTH_IN_SAMPLES 1
