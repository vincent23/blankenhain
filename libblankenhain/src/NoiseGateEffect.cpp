#include "NoiseGateEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "BhMath.h"

NoiseGateEffect::NoiseGateEffect()
	: EffectBase(5u, false)
	, lookaheadBuffer(static_cast<unsigned int>(aux::millisecToSamples(5.1f)))
	, RMSVolLength(static_cast<unsigned int>(aux::millisecToSamples(50.f)))
	, lastStartTime(0u)
	, lastStopTime(0u)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(-6.f, NormalizedRange(-120.f, 0.f, 5.f), "threshold", "dB");
  (params->getParameter(1)) = new FloatParameter(-119.f, NormalizedRange(-120.f, 0.f, 5.f), "bottom", "dB");
	(params->getParameter(2)) = new FloatParameter(1.f, NormalizedRange(0.05f, 200.f, 0.8f), "attack", "ms");
	(params->getParameter(3)) = new FloatParameter(1.f, NormalizedRange(0.05f, 200.f, 0.8f), "release", "ms");
	(params->getParameter(4)) = new FloatParameter(0.f, NormalizedRange(0.0f, 5.f, 0.8f), "lookahead", "ms");
}

void NoiseGateEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	const float thresholdInDb = getInterpolatedParameter(0).get();
  const float bottomInDb = getInterpolatedParameter(1).get();
	const float attackInMs = getInterpolatedParameter(2).get();
	const float releaseInMs = getInterpolatedParameter(3).get();
	const float lookaheadnMs = getInterpolatedParameter(4).get();
	const unsigned int lookaheadInSamples = static_cast<unsigned int>(aux::millisecToSamples(lookaheadnMs));
	const unsigned int delayLineLength = RMSVolLength + lookaheadInSamples;

	lookaheadBuffer.setSize(delayLineLength);
	

	for (size_t bufferIteration = 0u; bufferIteration < numberOfSamples; bufferIteration++)
	{
		Sample BufferOut = lookaheadBuffer.pushpop(buffer[bufferIteration]);
		RMSkeeper -= BufferOut * BufferOut;
		RMSkeeper += buffer[bufferIteration] * buffer[bufferIteration];

		const Sample RMScurrent = (RMSkeeper / Sample(static_cast<float>(delayLineLength))).sqrt();

    buffer[bufferIteration] = BufferOut;

		if (RMScurrent.maxValue() > aux::decibelToLinear(thresholdInDb))
		{
			if (lastStopTime < lastStartTime)
        lastStopTime = currentTime;

			if (currentTime - lastStopTime < aux::millisecToSamples(releaseInMs))
			{
          // perform release, gate just turned off
        float curFloatVolume = 1.f - aux::decibelToLinear(bottomInDb);
        curFloatVolume /= static_cast<float>(aux::millisecToSamples(releaseInMs));
        curFloatVolume *= static_cast<float>(currentTime - lastStartTime);
        Sample currentVolume(curFloatVolume + aux::decibelToLinear(bottomInDb));
        buffer[bufferIteration] *= currentVolume;
      }
			else
			{
        // Gate not active
      }
		}
		else if (currentTime - lastStartTime < aux::millisecToSamples(attackInMs))
		{
			if (lastStartTime < lastStopTime)
				lastStartTime = currentTime;
			// perform attack, gate just turned on
      float curFloatVolume = 1.f - aux::decibelToLinear(bottomInDb);
      curFloatVolume /= static_cast<float>(aux::millisecToSamples(attackInMs));
      curFloatVolume *= static_cast<float>(aux::millisecToSamples(attackInMs) - currentTime - lastStartTime);
      Sample currentVolume(curFloatVolume + aux::decibelToLinear(bottomInDb));
      buffer[bufferIteration] *= currentVolume;
		}
		else
		{
      if (lastStartTime < lastStopTime)
        lastStartTime = currentTime;
			// quiet, gate active
      buffer[bufferIteration] *= Sample(aux::decibelToLinear(bottomInDb));
		}
    currentTime++;
		nextSample();
	}
}