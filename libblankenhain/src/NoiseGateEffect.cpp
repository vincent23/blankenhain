#include "NoiseGateEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "BhMath.h"

NoiseGateEffect::NoiseGateEffect()
	: EffectBase(6u, false)
	//, lookaheadBuffer(static_cast<unsigned int>(aux::millisecToSamples(60.2f)))
	, lookaheadBuffer(1 << 12)
	, RMSVolLength(static_cast<unsigned int>(aux::millisecToSamples(1.f)))
	, lastStartTime(1u)
	, lastStopTime(0u)
{
	ParameterBundle& params = getParameterBundle();
	params.initParameter(0, new FloatParameter(-6.f, NormalizedRange(-120.f, 0.f, 5.f), "threshold", "dB"));
	params.initParameter(1, new FloatParameter(-119.f, NormalizedRange(-120.f, 0.f, 5.f), "bottom", "dB"));
	params.initParameter(2, new FloatParameter(1.f, NormalizedRange(0.05f, 300.f, 0.8f), "attack", "ms"));
	params.initParameter(3, new FloatParameter(1.f, NormalizedRange(0.05f, 300.f, 0.8f), "release", "ms"));
	params.initParameter(4, new FloatParameter(20.f, NormalizedRange(0.0f, 99.f, 0.8f), "lookahead", "ms"));
	params.initParameter(5, new FloatParameter(10.f, NormalizedRange(0.0f, 1000.f, 0.8f), "hold", "ms"));
}

void NoiseGateEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	const float threshold = aux::decibelToLinear(interpolatedParameters.get(0));
	const float bottom = aux::decibelToLinear(interpolatedParameters.get(1));
	const float attack = aux::millisecToSamples(interpolatedParameters.get(2));
	const float release = aux::millisecToSamples(interpolatedParameters.get(3));
	const float lookahead = aux::millisecToSamples(interpolatedParameters.get(4));
	const float hold = aux::millisecToSamples(interpolatedParameters.get(5));

	const unsigned int lookaheadInSamples = static_cast<unsigned int>(lookahead);
	const unsigned int delayLineLength = RMSVolLength + lookaheadInSamples;

	this->delayEffectProducesInSamples = delayLineLength;

	// lookaheadBuffer.setSize(delayLineLength);
	

	for (size_t bufferIteration = 0u; bufferIteration < numberOfSamples; bufferIteration++)
	{
		//Sample BufferOut = lookaheadBuffer.pushpop(buffer[bufferIteration]);
		lookaheadBuffer.push(buffer[bufferIteration]);
		Sample BufferOut = lookaheadBuffer.get(delayLineLength);
		RMSkeeper -= BufferOut * BufferOut;
		RMSkeeper += buffer[bufferIteration] * buffer[bufferIteration];

		const Sample RMScurrent = (RMSkeeper / Sample(static_cast<float>(delayLineLength))).sqrt();

		buffer[bufferIteration] = BufferOut;



		if (RMScurrent.maxValue() > threshold)
		{
			if (lastStartTime <= lastStopTime)
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
				// Gate not active
			}
		}
		else
		{
			if (lastStopTime < lastStartTime)
				lastStopTime = currentTime;

			if (currentTime - lastStopTime < release + hold)
			{
				// perform hold and release
				float curBottom = bottom;
				if (currentTime - lastStopTime < hold)
					curBottom = 1.f;

				// perform release, gate just turned off
				float curFloatVolume = 1.f - curBottom;
				curFloatVolume /= static_cast<float>(release);
				curFloatVolume *= static_cast<float>(release - (currentTime - lastStopTime - hold));
				Sample currentVolume(curBottom + curFloatVolume);
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