#include "processFunctions.h"
#include "AuxFunc.h"
#include <cmath>
#include <algorithm>

namespace effects
{
	namespace pan
	{
		// Channel number is expected to be == 2
		// CurrentPanning between -50 and 50
		void processPanning(Sample* data, size_t numberOfSamples, float panningValue, bool mono)
		{
			if (mono)
			{
				for (size_t bufferIteration = 0u; bufferIteration < numberOfSamples; bufferIteration++)
				{
					alignas(16) double lr[2];
					data[bufferIteration].store_aligned(lr);
					lr[1] = lr[0] * (1.f + std::min(0.0f, panningValue) * 0.02f);
					lr[0] = lr[0] * (1.f - std::max(0.0f, panningValue) * 0.02f);
					data[bufferIteration] = load_aligned(lr);
				}
			}
			else
			{
				for (size_t bufferIteration = 0u; bufferIteration < numberOfSamples; bufferIteration++)
				{
					alignas(16) double lr[2];
					data[bufferIteration].store_aligned(lr);
					lr[0] = lr[0] * (1.f - std::max(0.0f, panningValue) * 0.02f);
					lr[1] = lr[1] * (1.f + std::min(0.0f, panningValue) * 0.02f);
					data[bufferIteration] = load_aligned(lr);
				}
			}
		}
	}

	namespace bitcrush
	{
		// Casts value of sample to int (discretization)
		// and back to double
		Sample discretize(Sample const& sample)
		{
			alignas(16) double lr[2];
			sample.store_aligned(lr);
			lr[0] = static_cast<double>(static_cast<int>(lr[0]));
			lr[1] = static_cast<double>(static_cast<int>(lr[1]));
			return load_aligned(lr);
		}

		// Channel number is expected to be == 2
		// All parameter values between 0 and 1
		void processBitcrush(Sample* data, size_t numberOfSamples, float crush, float downsample,
			float wet)
		{
			int groupedSamples = std::min(static_cast<int>(std::max(1.f, downsample * 100.f)), static_cast<int>(numberOfSamples));
			float bitdepth = 12.f * (1.f - crush) + 1.f * crush;
			int steps = static_cast<int>(exp2(bitdepth));

			for (size_t sample = 0; sample < numberOfSamples - groupedSamples; sample += groupedSamples)
			{
				Sample averagedSample(0., 0.);
				for (int i = 0; i < groupedSamples; i++)
				{
					averagedSample += data[i + sample] / Sample((double)groupedSamples);
				}

				Sample discretizedSample = discretize(averagedSample * Sample(static_cast<double>(steps)));
				discretizedSample /= Sample(static_cast<double>(steps));

				for (int i = 0; i < groupedSamples; i++) {
					Sample sampleValue = data[i + sample];
					data[i + sample] = sampleValue * Sample(1. - wet) + discretizedSample * Sample(wet);
				}
			}

			Sample averagedSample(0., 0.);
			for (size_t i = static_cast<size_t>(numberOfSamples / groupedSamples) * groupedSamples; i < numberOfSamples; i++)
			{
				averagedSample += data[i] / Sample(static_cast<double>((numberOfSamples % groupedSamples)));
			}

			Sample discretizedSample = discretize(averagedSample * Sample(static_cast<double>(steps)));
			discretizedSample /= Sample(static_cast<double>(steps));

			for (size_t i = static_cast<size_t>(numberOfSamples / groupedSamples) * groupedSamples; i < numberOfSamples; i++)
			{
				Sample sampleValue = data[i];
				data[i] = sampleValue * Sample(1. - wet) + discretizedSample * Sample(wet);
			}
		}
	}

	namespace compressor
	{
		// Channel number is expected to be == 2
		// All parameter values between 0 and 1
		void processCompressor(Sample* data, size_t numberOfSamples,
			float release, float attack, float ratio, float threshold,
			bool limiterOn,
			CircularBuffer<Sample>& delayLine, Sample& envelope)
		{
			attack = attack / 1000.f;
			size_t attackTimeInSamples = static_cast<size_t>(attack * 44100);
			delayLine.setSize(attackTimeInSamples);
			release = release / 1000.f;
			float attackGain = exp(-1 / (attack * 44100));
			float releaseGain = exp(-1 / (release * 44100));
			float slope = 1 - (1 / ratio);
			for (size_t i = 0; i < numberOfSamples; i++)
			{
				Sample input = abs(data[i]);
				if (envelope.areBothSmaller(input))
				{
					envelope = input + Sample(static_cast<double>(attackGain)) * (envelope - input);
				}
				else
				{
					envelope = input + Sample(static_cast<double>(releaseGain)) * (envelope - input);
				}
				double envelopeValue = maxValue(envelope);
				float envelopeDb = aux::linearToDecibel(static_cast<float>(abs(envelopeValue)));
				if (envelopeDb < threshold)
				{
					envelopeDb = 0.f;
				}
				else
				{
					if (limiterOn)
					{
						envelopeDb = (threshold - envelopeDb);
					}
					else envelopeDb = slope * (threshold - envelopeDb);
				}
				data[i] = delayLine.pushpop(data[i]);
				data[i] *= Sample(static_cast<double>(aux::decibelToLinear(envelopeDb)));
			}
		}
	}

	namespace delay
	{
		// Pan between -1 and 1
		void processDelay(Sample* data, size_t numberOfSamples,
			float pan, float length, float feedback, float drywet,
			CircularBuffer<Sample>& delayline)
		{
			delayline.setSize(static_cast<size_t>(aux::millisecToSamples(length)));
			for (size_t i = 0; i < numberOfSamples; i++)
			{
				double avg_ = avgValue(data[i]);
				Sample original = data[i];
				Sample line = delayline.get();

				if (drywet > 0.5)
				{
					original *= Sample((1 - drywet) * 2.f);
				}
				else line *= Sample(drywet * 2.f);

				// Pan
				alignas(16) double lr[2];
				line.store_aligned(lr);
				lr[0] = lr[0] * (1.f - std::max(0.0f, pan));
				lr[1] = lr[1] * (1.f + std::min(0.0f, pan));
				line = load_aligned(lr);

				data[i] = original + line;

				delayline.push((delayline.get()) * Sample(feedback) + Sample(avg_));
			}
		}
	}

	namespace distortion
	{
		void processNonlinDistortion(Sample* data, size_t numberOfSamples, size_t algorithm, float ingain, size_t iterations)
		{
			if (algorithm == 0) return;
			else
			{
				// InGain
				for (size_t i = 0; i < numberOfSamples; i++) data[i] *= Sample(aux::decibelToLinear(ingain));

				//Upsample
				Sample* upsampledBuffer = upsampleZeros(data, numberOfSamples, 8u);

				// DISTORTION STARTS //
				// Take care, that all values are between -1´and 1
				// Values beyond this (so >0db) will fuck up the nonlinearity function
				// All linearities via http://www.mitpressjournals.org/doi/pdf/10.1162/comj.2009.33.2.85

				//Iterations through nonlinear scaling
				for (size_t j = 0; j < iterations; j++)
				{
					// Araya and Suyama
					if (algorithm == 1u)
					{
						for (size_t i = 0; i < 8 * numberOfSamples; i++)
						{
							upsampledBuffer[i] = upsampledBuffer[i] * Sample(1.5) \
								* (Sample(1.) - upsampledBuffer[i] * upsampledBuffer[i] / Sample(3.));
						}
					}
					// Doidic et al. symmetric
					else if (algorithm == 2u)
					{
						for (size_t i = 0; i < 8 * numberOfSamples; i++)
						{
							upsampledBuffer[i] =
								(abs(Sample(2.) *  upsampledBuffer[i]) \
									- upsampledBuffer[i] * upsampledBuffer[i]) \
								* sign(upsampledBuffer[i]);
						}
					}
					// Doidic et al. asymmetric
					else if (algorithm == 3u)
					{
						for (size_t i = 0; i < 8 * numberOfSamples; i++)
						{
							alignas(16) double lr[2];
							upsampledBuffer[i].store_aligned(lr);
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
							upsampledBuffer[i].load_aligned(lr);
						}
					}
				}

				// Lowpass
				// This is very confusing, the lowpass seems to make the sound only worse.
				// However, without lowPassing the whole Upsampling
				// procedure is useless. might es well let it be.
				//
				// effects::filter::Filter filter;
				// filter.processLow(upsampledBuffer, 8 * numberOfSamples, 40000., 0.1, 8*44100);

				// Downsample
				Sample *newData = simpleDownsample(upsampledBuffer, 8 * numberOfSamples, 8u);

				// Copy (note: maybe we could also handle **Sample, then copying would not be neccessary)
				for (size_t i = 0; i < numberOfSamples; i++)
				{
					data[i] = newData[i];
				}

				// Cleanup
				delete[] upsampledBuffer;
				delete[] newData;
			}
		}
	}
}