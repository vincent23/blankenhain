#include "NoiseGateEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "BhMath.h"

NoiseGateEffect::NoiseGateEffect()
	: EffectBase(6u, false)
	, lookaheadBuffer(static_cast<unsigned int>(aux::millisecToSamples(5.1f)))
	, RMSVolLength(static_cast<unsigned int>(aux::millisecToSamples(50.f)))
	, lastStartTime(1u)
	, lastStopTime(0u)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(-6.f, NormalizedRange(-120.f, 0.f, 5.f), "threshold", "dB");
	(params->getParameter(1)) = new FloatParameter(-119.f, NormalizedRange(-120.f, 0.f, 5.f), "bottom", "dB");
	(params->getParameter(2)) = new FloatParameter(50.f, NormalizedRange(0.f, 100.f, 5.f), "rmsVolLength", "ms");
	(params->getParameter(3)) = new FloatParameter(1.f, NormalizedRange(0.05f, 200.f, 0.8f), "attack", "ms");
	(params->getParameter(4)) = new FloatParameter(1.f, NormalizedRange(0.05f, 200.f, 0.8f), "release", "ms");
	(params->getParameter(5)) = new FloatParameter(0.f, NormalizedRange(0.0f, 5.f, 0.8f), "lookahead", "ms");
}

void NoiseGateEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	const float threshold = aux::decibelToLinear(getInterpolatedParameter(0).get());
	const float bottom = aux::decibelToLinear(getInterpolatedParameter(1).get());
	const float attack = aux::millisecToSamples(getInterpolatedParameter(3).get());
	const float release = aux::millisecToSamples(getInterpolatedParameter(4).get());
	const float lookahead = aux::millisecToSamples(getInterpolatedParameter(5).get());
	const unsigned int lookaheadInSamples = static_cast<unsigned int>(lookahead);
	const unsigned int delayLineLength = static_cast<unsigned int>(aux::millisecToSamples(getInterpolatedParameter(2).get()))
		+ lookaheadInSamples;

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
		nextSample();
	}
}