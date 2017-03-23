#pragma once

class ParameterTrack
{
public:
	ParameterTrack(unsigned int numberOfPoints, unsigned int* samplePositions, float* normalizedValues);
	float getCurrentValueAndAdvance(unsigned int samplePosition);
	float getCurrentDiscreteValueAndAdvance(unsigned int samplePosition);

	unsigned int numberOfPoints;
	unsigned int* samplePositions;
	float* normalizedValues;
	unsigned int currentPointIndex = 0;
};