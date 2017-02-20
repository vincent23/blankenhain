#include "DistortionEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "BhMath.h"

DistortionEffect::DistortionEffect() : EffectBase(4)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(0.f, NormalizedRange(-12.f, 12.f, 1.f), "inGain", "dB");
	(params->getParameter(1)) = new FloatParameter(0.f, NormalizedRange(0.f, 10.f, 1.f), "iterations", "");
	(params->getParameter(2)) = new FloatParameter(1.f, NormalizedRange(), "algorithm", "");
	(params->getParameter(3)) = new FloatParameter(0.f, NormalizedRange(0.f, 1.f, 0.3f), "dry/wet", "skewed");
}

void DistortionEffect::process(Sample* buffer, size_t numberOfSamples)
{
	float inGain = getInterpolatedParameter(0).get();
	float drywet = getInterpolatedParameter(3).get();
	size_t iterations = static_cast<size_t>(getInterpolatedParameter(1).get());


	distortionAlgorithms algo;
	if (getInterpolatedParameter(2).get() < 0.3333) algo = distortionAlgorithms::ArayaAndSuyama;
	else if (getInterpolatedParameter(2).get() < 0.66666) algo = distortionAlgorithms::DoidicSymmetric;
	else algo = distortionAlgorithms::DoidicAsymmetric;

	alignas(16) double lr[2];
	//Iterations through nonlinear scaling
	Sample inGainSample(aux::decibelToLinear(inGain));
	for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
	{
		Sample processed(buffer[bufferIteration]);
		processed *= inGainSample;
		if (algo == distortionAlgorithms::ArayaAndSuyama)
		{
			for (unsigned int j = 0; j < iterations; j++) {
				processed = processed * (Sample(1.5) - (processed * processed) * Sample(.5));
			}
		}
		else if (algo == distortionAlgorithms::DoidicSymmetric)
		{
			for (unsigned int j = 0; j < iterations; j++) {
				processed = (Sample(2.) * processed.abs() - processed * processed) * processed.sign();
			}
		}
		else
		{
			processed.store_aligned(lr);
			for (unsigned int j = 0; j < iterations; j++) {
				// Treat both channels (l / r) seperately
				for (size_t k = 0; k < 2u; k++)
				{
					if (lr[k] < -0.08905f)
					{
						//lr[k] = -0.75 * (1. - static_cast<double>(BhMath::pow(1.f - (BhMath::abs(static_cast<float>(lr[k])) - 0.032847f), 12.f))
						//	+ (1. / 3.) * (static_cast<double>(BhMath::abs(static_cast<float>(lr[k]))) - 0.032847f)) + 0.01;
						// optimized version of the above
						float in = BhMath::abs(static_cast<float>(lr[k])) - 0.032847f;
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
			processed = Sample::load_aligned(lr);
		}
		buffer[bufferIteration] = aux::mixDryWet(buffer[bufferIteration], processed, drywet);
	}
	nextSample(numberOfSamples);
}