#include "DistortionEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

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
	InterpolatedValue<float>& inGain = getInterpolatedParameter(0);
	InterpolatedValue<float>& drywet = getInterpolatedParameter(3);
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
		processed *= Sample(aux::decibelToLinear(inGain.get(bufferIteration)));
		for (size_t j = 0; j < iterations; j++)
		{
			if (algo == distortionAlgorithms::ArayaAndSuyama)
			{
				processed = processed * Sample(1.5) * (Sample(1.) - processed * processed / Sample(3.));
			}
			else if (algo == distortionAlgorithms::DoidicSymmetric)
			{
				processed =
					((Sample(2.) *  processed).abs() \
						- processed * processed) \
					* processed.sign();
			}
			else
			{
				processed.store_aligned(lr);
				// Treat both channels (l / r) seperately
				for (size_t k = 0; k < 2u; k++)
				{
					if (lr[k] < -0.08905)
					{
						lr[k] = -0.75 * (1. - std::pow(1. - (std::abs(lr[k]) - 0.032847), 12) \
							+ (1. / 3.) * (std::abs(lr[k]) - 0.032847)) + 0.01;
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
				processed.load_aligned(lr);
			}
		}
		if (drywet.get() > 0.5)
		{
			original *= Sample((1 - drywet.get()) * 2.f);
		}
		else 
			processed *= Sample(drywet.get() * 2.f);
		buffer[bufferIteration] = original + processed;
	}
	nextSample(numberOfSamples);
}