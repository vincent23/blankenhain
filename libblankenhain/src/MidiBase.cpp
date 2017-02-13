#include "MidiBase.h"

void MidiBase::process(Sample* buffer, size_t numberOfSamples)
{
	this->nextSample(numberOfSamples);
}

bool MidiBase::startNote(unsigned int key, unsigned int duration)
{
	if (numberOfNotesInFlight >= constants::maxMidiNotesInFlight) {
		return false;
	}

	// look for the right index for the new note in the sorted sequence
	unsigned int endPosition = currentSamplePosition + duration;
	unsigned int insertionIndex = 0;
	while (insertionIndex < numberOfNotesInFlight
		&& endPositions[getPosition(insertionIndex)] < endPosition)
	{
		insertionIndex++;
	}

	// insert new node
	unsigned insertionPosition = getPosition(insertionIndex);
	unsigned int oldEndPosition = endPositions[insertionIndex];
	unsigned int oldKey = keys[insertionIndex];
	endPositions[insertionPosition] = endPosition;
	keys[insertionPosition] = key;
	numberOfNotesInFlight++;

	// move old notes which come after the note
	for (unsigned int noteIndex = insertionIndex + 1; noteIndex < numberOfNotesInFlight; noteIndex++)
	{
		unsigned int notePosition = getPosition(noteIndex);
		unsigned int tempEndPosition = endPositions[notePosition];
		unsigned int tempKey = keys[notePosition];
		endPositions[notePosition] = oldEndPosition;
		keys[notePosition] = oldKey;
		oldEndPosition = tempEndPosition;
		oldKey = tempKey;
	}
	return true;
}

void MidiBase::skipSamples(unsigned int samples)
{
	currentSamplePosition += samples;
}

bool MidiBase::advanceToNextStopNote(unsigned int maxSamples, unsigned int& outSamplesAdvanced, unsigned int& outKey)
{
	outSamplesAdvanced = maxSamples;
	if (numberOfNotesInFlight == 0) {
		currentSamplePosition += outSamplesAdvanced;
		return false;
	}

	unsigned int firstPosition = getPosition(0);
	unsigned int samplesUntilNextEnd = endPositions[firstPosition] - currentSamplePosition;
	if (samplesUntilNextEnd <= maxSamples) {
		outKey = keys[firstPosition];
		outSamplesAdvanced = samplesUntilNextEnd;
		currentSamplePosition += outSamplesAdvanced;
		numberOfNotesInFlight--;
		inFlightStartPosition = getPosition(1);
		return true;
	}
	else
	{
		currentSamplePosition += outSamplesAdvanced;
		return false;
	}
}

unsigned int MidiBase::getPosition(unsigned int index) const
{
	return (index % constants::maxMidiNotesInFlight) + inFlightStartPosition;
}