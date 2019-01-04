#include "DistortionEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "BhMath.h"

DistortionEffect::DistortionEffect() : EffectBase(4)
{
	ParameterBundle& params = getParameterBundle();
	params.initParameter(0, new FloatParameter(0.f, NormalizedRange(-12.f, 12.f, 1.f), "inGain", "dB"));
	params.initParameter(1, new FloatParameter(0.f, NormalizedRange(0.f, 10.f, 1.f), "Intensity", "")); // Is number of iterations the nonlin. transform. is applied
	const BhString distorStyles[3] = { "Araya & Suyama", "Doidic - Symmetric", "Doidic - Asymmetric" };
	params.initParameter(2, new OptionParameter(3u, distorStyles, "Algorithm", ""));
	params.initParameter(3, new FloatParameter(0.f, NormalizedRange(0.f, 1.f, 0.3f), "dry/wet", "skewed"));
}

void DistortionEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	const float inGain = interpolatedParameters.get(0);
	const float drywet = interpolatedParameters.get(3);
	const float iterations = interpolatedParameters.get(1);
	const size_t iterationsRoundedUp = static_cast<size_t>(iterations) + 1u;


	distortionAlgorithms algo;
	if (interpolatedParameters.get(2) < 1.f) algo = distortionAlgorithms::ArayaAndSuyama;
	else if (interpolatedParameters.get(2) < 2.f) algo = distortionAlgorithms::DoidicSymmetric;
	else algo = distortionAlgorithms::DoidicAsymmetric;

	Sample input[constants::blockSize];
	for (unsigned int i = 0; i < numberOfSamples; i++) {
		input[i] = buffer[i];
	}

	//Iterations through nonlinear scaling
	Sample inGainSample(aux::decibelToLinear(inGain));
	if (algo == distortionAlgorithms::ArayaAndSuyama)
	{
		for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
		{
			Sample& processed = buffer[bufferIteration];
			processed *= inGainSample;
			Sample lastValue;
			for (unsigned int j = 0; j < iterationsRoundedUp; j++) 
			{
				lastValue = processed;
				processed = processed * (Sample(1.5) - (processed * processed) * Sample(.5));
			}
			processed *= Sample(1.f - (static_cast<float>(iterationsRoundedUp) - iterations));
			processed += lastValue * Sample(static_cast<float>(iterationsRoundedUp) - iterations);
		}
	}
	else if (algo == distortionAlgorithms::DoidicSymmetric)
	{
		for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
		{
			Sample& processed = buffer[bufferIteration];
			processed *= inGainSample;
			Sample lastValue;
			for (unsigned int j = 0; j < iterationsRoundedUp; j++) {
				lastValue = processed;
				processed = (Sample(2.) * processed.abs() - processed * processed) * processed.sign();
			}
			processed *= Sample(static_cast<float>(iterationsRoundedUp) - iterations);
			processed += lastValue * Sample(1.f - (static_cast<float>(iterationsRoundedUp) - iterations));
		}
	}
	else
	{
		alignas(16) floatType lr[2];
		for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
		{
			Sample& processed = buffer[bufferIteration];
			processed *= inGainSample;
			Sample lastValue;
			processed.store_aligned(lr);
			for (unsigned int j = 0; j < iterationsRoundedUp; j++)
			{
				lastValue = Sample(lr[0], lr[1]);
				// Treat both channels (l / r) seperately
				for (size_t k = 0; k < 2u; k++)
				{
					if (lr[k] < -0.08905f)
					{
						//lr[k] = -0.75 * (1. - static_cast<double>(BhMath::pow(1.f - (BhMath::abs(static_cast<float>(lr[k])) - 0.032847f), 12.f))
						//	+ (1. / 3.) * (static_cast<double>(BhMath::abs(static_cast<float>(lr[k]))) - 0.032847f)) + 0.01;
						// optimized version of the above
						const float in = BhMath::abs(static_cast<float>(lr[k])) - 0.032847f;
						float base = 1.f - in;
						base *= base; // ^2
						base *= base; // ^4
						base = base * base * base; // ^12
						lr[k] = -0.74f + .75f * base - .25f * in;
					}
					else if (lr[k] < 0.320018)
					{
						lr[k] = -6.153 * (lr[k] * lr[k]) + 3.9375 * lr[k];
					}
					else
					{
						lr[k] = 0.630035;
					}
				}
			}
			processed = Sample(lr[0], lr[1]);
			processed *= Sample(1.f - (static_cast<float>(iterationsRoundedUp) - iterations));
			processed += lastValue * Sample((static_cast<float>(iterationsRoundedUp) - iterations));
		}
	}

	for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
	{
		buffer[bufferIteration] = aux::mixDryWet(input[bufferIteration], buffer[bufferIteration], drywet);
	}
}