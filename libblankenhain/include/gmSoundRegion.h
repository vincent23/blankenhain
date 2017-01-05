#pragma once

/**
* Region data via dlParser. Only stores information, no processing here.
*/
class gmSoundRegion
{
public:
	unsigned int rootNote;
	unsigned int lowest;
	unsigned int highest;
	unsigned int startByte;
	// These three are in Byte! Therefore deivide by two if you need it in 16bit int 
	unsigned int sampleLength;
	unsigned int loopStart;
	unsigned int loopLength;
	bool isLoopable;
	gmSoundRegion()
		: rootNote(0u), lowest(0u), highest(0u), startByte(0u), sampleLength(0u), loopStart(0u), loopLength(0u), isLoopable(false) {};
};