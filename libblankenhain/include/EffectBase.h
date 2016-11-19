#pragma once

#include "Sample.h"

class ParameterBundle;

class EffectBase
{
public:
	EffectBase(unsigned int numberOfParameters, unsigned int const& blockSize_ = 128);
	virtual ~EffectBase();

	void processBlock(float* const* inputs, float* const* outputs, unsigned int bufferSize);
	ParameterBundle* getPointerToParameterBundle() const;
	unsigned int getNumberOfParameters() const;

	virtual void process(Sample* buffer, size_t sampleFrames, size_t numberOfParameters, float* parameters) = 0;

private:
	size_t blockSize;
	float* currentParameters;
	ParameterBundle* params;
	Sample* sseBuffer;
};