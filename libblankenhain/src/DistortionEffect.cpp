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


	alignas(16) double currentBuffer[2];
	double*const & lr = currentBuffer;
	//Iterations through nonlinear scaling
	for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
	{

		Sample processed(buffer[bufferIteration]), original(buffer[bufferIteration]);
		processed *= Sample(aux::decibelToLinear(inGain));
		for (size_t j = 0; j < iterations; j++)
		{
			if (algo == distortionAlgorithms::ArayaAndSuyama)
			{
				processed = processed * Sample(1.5) * (Sample(1.) - processed * processed / Sample(3.));
			}
			else if (algo == distortionAlgorithms::DoidicSymmetric)
			{
				processed =
					(Sample(2.) *  processed.abs()
						- processed * processed)
					* processed.sign();
			}
			else
			{
				processed.store_aligned(lr);
				// Treat both channels (l / r) seperately
				for (size_t k = 0; k < 2u; k++)
				{
					if (lr[k] < -0.08905f)
					{
						lr[k] = -0.75 * (1. - static_cast<double>(BhMath::pow(1.f - ( BhMath::abs(static_cast<float>(lr[k])) - 0.032847f ), 12.f))
							+ (1. / 3.) * (static_cast<double>(BhMath::abs(static_cast<float>(lr[k]))) - 0.032847f)) + 0.01;
					}
					else if (lr[k] < 0.320018)
					{
						lr[k] = -6.153 * lr[k] * lr[k] + 3.9375 * lr[k];
					}
					else
					{
						lr[k] = 0.630035;
					}
				}
				processed = Sample::load_aligned(lr);
			}
		}
		if (drywet > 0.5)
		{
			original *= Sample((1.f - drywet) * 2.f);
		}
		else
		{
			processed *= Sample(drywet * 2.f);
		}
		buffer[bufferIteration] = original + processed;
	}
	nextSample(numberOfSamples);
}