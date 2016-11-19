#include "EffectBase.h"
#include "ParameterBundle.h"

// for constants::blockSize
#include "processFunctions.h"

template<size_t BlockSize, typename ProcessFunction>
static void processBlockwise(float* const* audioBufferIn, float* const* audioBufferOut, size_t numberOfSamples,
	Sample* sseBuffer, size_t numberOfParameters, float* currentParameters, size_t interpolaionDistance, ProcessFunction processFunction) {
	// Main Loop, performed till AudioBufferFloats are less than an integer multiple of Blocksize
	size_t offset = interpolaionDistance;
	for (; static_cast<int>(offset + BlockSize) <= numberOfSamples; offset += BlockSize)
	{
		// This fills SIMD-Sample-Array of size "Blocksize" from AudioBuffer
		for (size_t i = 0; i < BlockSize; i++) {
			int sampleIndex = offset + i;
			sseBuffer[i] = Sample(audioBufferIn[0][sampleIndex], audioBufferIn[1][sampleIndex]);
		}

		//


		// Now, audio processing is performed
		processFunction(sseBuffer, BlockSize, offset, numberOfParameters, currentParameters);
		alignas(16) double sampleValues[2];
		// And data is written back from SIMD-Sample-Array to AudioBuffer
		for (size_t i = 0; i < BlockSize; i++) {
			int sampleIndex = offset + i;
			sseBuffer[i].store_aligned(sampleValues);
			audioBufferOut[0][sampleIndex] = float(sampleValues[0]);
			audioBufferOut[1][sampleIndex] = float(sampleValues[1]);
		}
	}

	// Take care of the remaining samples
	const size_t remainingSamples = numberOfSamples - offset;
	if (remainingSamples != 0u)
	{
		for (size_t i = 0; i < remainingSamples; i++) {
			int sampleIndex = offset + i;
			sseBuffer[i] = Sample(audioBufferIn[0][sampleIndex], audioBufferIn[1][sampleIndex]);
		}
		processFunction(sseBuffer, remainingSamples, offset, numberOfParameters, currentParameters);
		alignas(16) double sampleValues[2];
		for (size_t i = 0; i < remainingSamples; i++) {
			int sampleIndex = offset + i;
			sseBuffer[i].store_aligned(sampleValues);
			audioBufferOut[0][sampleIndex] = float(sampleValues[0]);
			audioBufferOut[1][sampleIndex] = float(sampleValues[1]);
		}
	}
}

EffectBase::EffectBase(unsigned int numberOfParameters, unsigned int const& blockSize_)
	: blockSize(blockSize_)
	, sseBuffer(new Sample[blockSize])
	, params(new ParameterBundle(numberOfParameters))
	, currentParameters(new float[numberOfParameters])
{ }

EffectBase::~EffectBase()
{
	if (sseBuffer != nullptr) delete[] sseBuffer;
	sseBuffer = nullptr;

	for (size_t i = 0u; i < params->getNumberOfParameters(); i++) {
		if (params->getParameter(i) != nullptr) delete params->getParameter(i);
		params->getParameter(i) = nullptr;
	}
	if (params != nullptr) delete params;
	params = nullptr;
	if (currentParameters != nullptr) delete[] currentParameters;
	currentParameters = nullptr;
}

void EffectBase::processBlock(float* const* inputs, float* const* outputs, unsigned int bufferSize)
{

	bool willBeInterpolated = false;
	unsigned int interpolationDistance = 0u;

	for (unsigned int j = 0u; j < this->params->getNumberOfParameters(); j++)
	{
		if (!this->params->getParameter(j)->oldAndTargetValueMatch())
		{
			willBeInterpolated = true; break;
		}
	}

	if (willBeInterpolated)
	{

		interpolationDistance = blockSize < bufferSize ? blockSize : bufferSize;

		for (size_t i = 0; i < interpolationDistance; i++) {
			sseBuffer[i] = Sample(inputs[0][i], inputs[1][i]);
		}

		for (unsigned int j = 0u; j < this->params->getNumberOfParameters(); j++)
		{
			this->params->getParameter(j)->setInterpolationDistance(blockSize);
		}


		alignas(16) double interpolationTempStorage[2];
		for (unsigned int i = 0u; i < interpolationDistance; i++)
		{
			for (unsigned int j = 0u; j < this->params->getNumberOfParameters(); j++)
			{
				this->currentParameters[j] = this->params->getParameter(j)->getImmediateValueAndUpdateUnnormalized();
			}
			this->process(sseBuffer + i, 1u, this->params->getNumberOfParameters(),
				this->currentParameters);
			sseBuffer[i].store_aligned(interpolationTempStorage);
			outputs[0][i] = float(interpolationTempStorage[0]);
			outputs[1][i] = float(interpolationTempStorage[1]);
		}

	}

	if (bufferSize - interpolationDistance != 0u)
	{
		for (unsigned int j = 0u; j < this->params->getNumberOfParameters(); j++)
		{
			this->currentParameters[j] = this->params->getParameter(j)->getTargetValueUnnormalized();
		}

		processBlockwise<constants::blockSize>(inputs, outputs, bufferSize, this->sseBuffer,
			params->getNumberOfParameters(), currentParameters, interpolationDistance,
			[this](Sample* sseBuffer_, size_t samples, size_t offset, size_t numberOfParam, float* currentParam)
		{
			this->process(sseBuffer_, samples, params->getNumberOfParameters(), currentParameters);
		}
		);
	}

	//Set current values as old values for interpolation in next buffer iteration
	if (willBeInterpolated)
	{
		for (size_t i = 0u; i < this->params->getNumberOfParameters(); i++)
		{
			ParameterWithProperties* parameter = this->params->getParameter(i);
			if (bufferSize - interpolationDistance != 0u)
			{
				parameter->setOldValueNormalized(parameter->getTargetValueNormalized());
				parameter->setImmediateValueNormalized(parameter->getTargetValueNormalized());
			}
			else
			{
				parameter->setOldValueNormalized(parameter->getImmediateValueNormalized());
			}
			parameter->resetInterpolationCounter();
		}
	}
}

ParameterBundle* EffectBase::getPointerToParameterBundle() const
{
	return (this->params);
}

unsigned int EffectBase::getNumberOfParameters() const
{
	return this->params->getNumberOfParameters();
}