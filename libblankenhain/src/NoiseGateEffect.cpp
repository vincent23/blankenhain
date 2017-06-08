#include "NoiseGateEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "BhMath.h"

NoiseGateEffect::NoiseGateEffect()
	: EffectBase(5u, false)
	, lookaheadBuffer(static_cast<unsigned int>(aux::millisecToSamples(60.2f)))
	, RMSVolLength(static_cast<unsigned int>(aux::millisecToSamples(10.f)))
	, lastStartTime(1u)
	, lastStopTime(0u)
{
	ParameterBundle& params = getParameterBundle();
	params.initParameter(0, new FloatParameter(-6.f, NormalizedRange(-120.f, 0.f, 5.f), "threshold", "dB"));
	params.initParameter(1, new FloatParameter(-119.f, NormalizedRange(-120.f, 0.f, 5.f), "bottom", "dB"));
	params.initParameter(2, new FloatParameter(1.f, NormalizedRange(0.05f, 200.f, 0.8f), "attack", "ms"));
	params.initParameter(3, new FloatParameter(1.f, NormalizedRange(0.05f, 200.f, 0.8f), "release", "ms"));
	params.initParameter(4, new FloatParameter(0.f, NormalizedRange(0.0f, 50.f, 0.8f), "lookahead", "ms"));
}

void NoiseGateEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	const float threshold = aux::decibelToLinear(interpolatedParameters.get(0));
	const float bottom = aux::decibelToLinear(interpolatedParameters.get(1));
	const float attack = aux::millisecToSamples(interpolatedParameters.get(2));
	const float release = aux::millisecToSamples(interpolatedParameters.get(3));
	const float lookahead = aux::millisecToSamples(interpolatedParameters.get(4));
	const unsigned int lookaheadInSamples = static_cast<unsigned int>(lookahead);
	const unsigned int delayLineLength = RMSVolLength + lookaheadInSamples;

	this->delayEffectProducesInSamples = delayLineLength;

	lookaheadBuffer.setSize(delayLineLength);
	

	for (size_t bufferIteration = 0u; bufferIteration < numberOfSamples; bufferIteration++)
	{
		Sample BufferOut = lookaheadBuffer.pushpop(buffer[bufferIteration]);
		RMSkeeper -= BufferOut * BufferOut;
		RMSkeeper += buffer[bufferIteration] * buffer[bufferIteration];

		const Sample RMScurrent = (RMSkeeper / Sample(static_cast<float>(delayLineLength))).sqrt();

		buffer[bufferIteration] = BufferOut;

		if (RMScurrent.maxValue() > threshold)
		{
			if (lastStopTime < lastStartTime)
			lastStopTime = currentTime;

			if (currentTime - lastStopTime < release)
			{
				// perform release, gate just turned off
				float curFloatVolume = 1.f - bottom;
				curFloatVolume /= static_cast<float>(release);
				curFloatVolume *= static_cast<float>(currentTime - lastStopTime);
				Sample currentVolume(curFloatVolume + bottom);
				buffer[bufferIteration] *= currentVolume;
			}
			else
			{
				// Gate not active
			}
		}
		else
		{
			if (lastStartTime < lastStopTime)
				lastStartTime = currentTime;
			
			if (currentTime - lastStartTime < attack)
			{
				// perform attack, gate just turned on
				float curFloatVolume = 1.f - bottom;
				curFloatVolume /= static_cast<float>(attack);
				curFloatVolume *= static_cast<float>(attack - (currentTime - lastStartTime));
				Sample currentVolume(curFloatVolume + bottom);
				buffer[bufferIteration] *= currentVolume;
			}
			else
			{
				// quiet, gate active
				buffer[bufferIteration] *= Sample(bottom);
			}
		}
		currentTime++;
	}
}