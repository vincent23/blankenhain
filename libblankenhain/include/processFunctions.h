#pragma once
#include "CircularBuffer.h"
#include "Sample.h"
#include "UpDownSampling.h"
#include <cmath>

namespace effects
{

	namespace pan
	{
		void processPanning(Sample* data, size_t numberOfSamples, float panningValue, bool mono);
	}

	namespace bitcrush
	{
		// Channel number is expected to be == 2
		// All parameter values between 0 and 1
		void processBitcrush(Sample* data, size_t numberOfSamples, float bitcrush, float downsample,
			float wet);

		// Casts value of sample to int (discretization)
		// and back to double
		Sample discretize(Sample const& sample);
	}

	namespace compressor
	{
		void processCompressor(Sample* data, size_t numberOfSamples,
			float release, float attack, float ratio, float threshold,
			bool limiterOn,
			CircularBuffer<Sample>& delayLine, Sample& envelope);
	}

	namespace delay
	{
		void processDelay(Sample* data, size_t numberOfSamples,
			float pan, float length, float feedback, float drywet,
			CircularBuffer<Sample>& delayline);
	}

	namespace distortion
	{
		void processNonlinDistortion(Sample *data, size_t numberOfSamples, size_t algorithm, float inGain, size_t iterations);
	}
}