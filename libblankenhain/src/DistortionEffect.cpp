#include "DistortionEffect.h"


#include "ParameterBundle.h"
#include "AuxFunc.h"
#include <cmath>

DistortionEffect::DistortionEffect() : EffectBase(3, 265)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(-12.f, 12.f, 1.f), "inGain", "dB");
	(params->getParameter(1)) = new ParameterWithProperties(0.f, NormalizedRange(0.f, 10.f, 1.f), "iterations", "");
	(params->getParameter(2)) = new ParameterWithProperties(1.f, NormalizedRange(), "algorithm", "");
}

void DistortionEffect::process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters)
{
	float& inGain = parameters[0];
	size_t iterations = static_cast<size_t>(parameters[1]);

	distortionAlgorithms algo;
	if (parameters[2] < 0.3333) algo = distortionAlgorithms::ArayaAndSuyama;
	else if (parameters[2] < 0.66666) algo = distortionAlgorithms::DoidicSymmetric;
	else algo = distortionAlgorithms::DoidicAsymmetric;


	// InGain
	for (size_t i = 0; i < numberOfSamples; i++) buffer[i] *= Sample(aux::decibelToLinear(inGain));

	alignas(16) double currentBuffer[2];
	double*const & lr = currentBuffer;
	//Iterations through nonlinear scaling
	for (size_t j = 0; j < iterations; j++)
	{
		if (algo == distortionAlgorithms::ArayaAndSuyama)
		{
			for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
			{
				buffer[bufferIteration] = buffer[bufferIteration] * Sample(1.5) * (Sample(1.) - buffer[bufferIteration] * buffer[bufferIteration] / Sample(3.));
			}
		}
		else if (algo == distortionAlgorithms::DoidicSymmetric)
		{
			for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
			{
				buffer[bufferIteration] =
					(abs(Sample(2.) *  buffer[bufferIteration]) \
						- buffer[bufferIteration] * buffer[bufferIteration]) \
					* sign(buffer[bufferIteration]);
			}
		}
		else
		{
			for (size_t i = 0; i < numberOfSamples; i++)
			{

				buffer[i].store_aligned(lr);
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
				buffer[i].load_aligned(lr);
			}
		}
	}
}