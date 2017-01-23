#include "ParameterTrack.h"

ParameterTrack::ParameterTrack(unsigned int numberOfPoints_, unsigned int* samplePositions_, float* normalizedValues_)
	: numberOfPoints(numberOfPoints_)
	, samplePositions(samplePositions_)
	, normalizedValues(normalizedValues_)
{}

float ParameterTrack::getCurrentValueAndAdvance(unsigned int samplePosition)
{
	// we alwyas have a point at samplePosition 0 (parser takes care of that)

	// advance currentPointIndex so that it points to the last point that is still before the samplePosition
	while (currentPointIndex + 1 < numberOfPoints && samplePosition < samplePositions[currentPointIndex + 1]) {
		currentPointIndex++;
	}

	float currentValue = normalizedValues[currentPointIndex];
	if (currentPointIndex + 1 < numberOfPoints) {
		// interpolate between this and the next point
		float nextValue = normalizedValues[currentPointIndex + 1];
		unsigned int currentPosition = samplePositions[currentPointIndex];
		unsigned int nextPosition = samplePositions[currentPointIndex + 1];
		unsigned int delta = nextPosition - currentPosition;
		unsigned int tSamples = samplePosition - nextPosition;
		float t = static_cast<float>(tSamples) / static_cast<float>(delta);
		return currentValue * (1.f - t) + nextValue * t;
	}
	else {
		// we have reached the end, just use the last value
		return currentValue;
	}
}