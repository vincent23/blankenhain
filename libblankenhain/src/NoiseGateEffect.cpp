#include "NoiseGateEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "BhMath.h"

NoiseGateEffect::NoiseGateEffect()
	: EffectBase(4u, false)
	, lookaheadBuffer(static_cast<unsigned int>(aux::millisecToSamples(5.1f)))
	, RMSVolLength(static_cast<unsigned int>(aux::millisecToSamples(50.f)))
	, lastStartTime(0u)
	, lastStopTime(0u)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(-6.f, NormalizedRange(-120.f, 0.f, 5.f), "threshold", "dB");
	(params->getParameter(1)) = new FloatParameter(1.f, NormalizedRange(0.05f, 200.f, 0.8f), "attack", "ms");
	(params->getParameter(2)) = new FloatParameter(1.f, NormalizedRange(0.05f, 200.f, 0.8f), "release", "ms");
	(params->getParameter(3)) = new FloatParameter(0.f, NormalizedRange(0.0f, 5.f, 0.8f), "lookahead", "ms");
}

void NoiseGateEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	using namespace BhMath;

	const float thresholdinDb = getInterpolatedParameter(0).get();
	const float attackInMs = getInterpolatedParameter(1).get();
	const float releaseInMs = getInterpolatedParameter(2).get();
	const float lookaheadnMs = getInterpolatedParameter(3).get();
	const unsigned int lookaheadInSamples = static_cast<unsigned int>(aux::millisecToSamples(lookaheadnMs));
	const unsigned int delayLineLength = RMSVolLength + lookaheadInSamples;

	lookaheadBuffer.setSize(delayLineLength);
	

	for (size_t bufferIteration = 0u; bufferIteration < numberOfSamples; bufferIteration++)
	{
		Sample BufferOut = lookaheadBuffer.pushpop(buffer[bufferIteration] * buffer[bufferIteration]);
		RMSkeeper -= BufferOut * BufferOut;
		RMSkeeper += buffer[bufferIteration] * buffer[bufferIteration];

		Sample RMScurrent = RMSkeeper / Sample(static_cast<float>(delayLineLength));
		RMScurrent = RMScurrent.sqrt();

		if (RMScurrent.maxValue() > aux::decibelToLinear(thresholdinDb))
		{
			if (lastStartTime < lastStopTime)
				lastStartTime = currentTime;

			if (currentTime - lastStartTime < aux::millisecToSamples(attackInMs))
			{
				// attack performance
			}
			else
			{
				//normal gate off
			}
		}
		else if (currentTime - lastStopTime < aux::millisecToSamples(releaseInMs))
		{
			if (lastStopTime < lastStartTime)
				lastStopTime = currentTime;
			// perform release
		}
		else
		{
			//quiet, gate active
		}

		//buffer[bufferIteration].store_aligned(currentBuffer);
		//currentBuffer[0] = currentBuffer[0] * aux::decibelToLinear(currentVolumeL.get());
		//currentBuffer[1] = currentBuffer[1] * aux::decibelToLinear(currentVolumeR.get());
		//buffer[bufferIteration] = Sample::load_aligned(currentBuffer);
		nextSample();
	}
}