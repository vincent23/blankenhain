#include "VibratoEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include <algorithm>

VibratoEffect::VibratoEffect() : EffectBase(4), delayLine(size_t(aux::millisecToSamples(2502u)))
{
	wasPaniced = false;
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(0.f, NormalizedRange(), "depth", "");
	(params->getParameter(1)) = new FloatParameter(100.f, NormalizedRange(0.1f, 2500.f), "time", "ms");
	(params->getParameter(2)) = new FloatParameter(1.f, NormalizedRange(), "drywet", "");
	(params->getParameter(3)) = new BoolParameter(false, "PANIC!");
}

void VibratoEffect::process(Sample* buffer, size_t numberOfSamples)
{
	InterpolatedValue<float>& depth = getInterpolatedParameter(0);
	InterpolatedValue<float>& time = getInterpolatedParameter(1);
	InterpolatedValue<float>& drywet = getInterpolatedParameter(2);
	bool panicButton = static_cast<bool>(getInterpolatedParameter(3).get());
	
	float delayLengthInMs = time.get();


	delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(delayLengthInMs)));

	if (panicButton && !wasPaniced)
	{
		wasPaniced = true;
		delayLine.reset();
	}
	else if (wasPaniced && !panicButton)
	{
		wasPaniced = false;
	}

	/*for (size_t i = 0; i < numberOfSamples; i++)
	{
		double avg_ = avgValue(buffer[i]);
		Sample original = buffer[i];
		Sample line = delayLine.get();

		if (drywet.get() > 0.5)
		{
			original *= Sample((1 - drywet.get()) * 2.f);
		}
		else line *= Sample(drywet.get() * 2.f);

		// Pan
		alignas(16) double lr[2];
		line.store_aligned(lr);
		lr[0] = lr[0] * (1.f - std::max(0.0f, pan.get()));
		lr[1] = lr[1] * (1.f + std::min(0.0f, pan.get()));
		line = load_aligned(lr);

		buffer[i] = original + line;

		delayLine.push(delayLine.get() * Sample(feedback.get()) + Sample(avg_));
		
		
		float lfoValue = lfo.getValue();
		int maxDelay = BASE_DELAY_SEC * sampleRate;

		float delay = lfoValue * depth * maxDelay;
		delay += 3;

		float value = buffer.getHermiteAt(delay);

		buffer.write_margined(input);

		return value;
		



		
		
		nextSample();




	}*/
}