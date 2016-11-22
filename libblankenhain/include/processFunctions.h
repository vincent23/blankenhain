#pragma once
#include "CircularBuffer.h"
#include "Sample.h"
#include "UpDownSampling.h"
#include <cmath>

namespace effects
{
	namespace filter
	{
		struct Filter : public AlignedType {
			Filter();

			void processHigh(Sample* data, size_t numberOfSamples, double frequency, double resonance, double sampleRate = 44100.);
			void processLow(Sample* data, size_t numberOfSamples, double frequency, double resonance, double sampleRate = 44100.);
			void processBand(Sample* data, size_t numberOfSamples, double frequency, double resonance, double sampleRate = 44100.);
			void processNotch(Sample* data, size_t numberOfSamples, double frequency, double resonance, double sampleRate = 44100.);
			void process(Sample* data, size_t numberOfSamples, double a1, double a2, double b0, double b1, double b2);
			void reset();

			Sample x_1, y_1, y_2;
		};
	}

	namespace pan
	{
		void processPanning(Sample* data, size_t numberOfSamples, float panningValue, bool mono);
	}

	namespace bitcrush
	{
		// Channel number is expected to be == 2
		// All parameter values between 0 and 1
		void processBitcrush(Sample* data, size_t numberOfSamples, float bitcrush, float downsample,
			float wet);

		// Casts value of sample to int (discretization)
		// and back to double
		Sample discretize(Sample const& sample);
	}

	namespace compressor
	{
		void processCompressor(Sample* data, size_t numberOfSamples,
			float release, float attack, float ratio, float threshold,
			bool limiterOn,
			CircularBuffer<Sample>& delayLine, Sample& envelope);
	}

	namespace delay
	{
		void processDelay(Sample* data, size_t numberOfSamples,
			float pan, float length, float feedback, float drywet,
			CircularBuffer<Sample>& delayline);
	}

	namespace equalizer
	{
		typedef struct alignas(16) : AlignedType
		{
			// Filter #1 (Low band)

			Sample lf; // Frequency
		  Sample f1p0; // Poles ...
		  Sample f1p1;
		  Sample f1p2;
		  Sample f1p3;

		  // Filter #2 (High band)

		  Sample hf; // Frequency
		  Sample f2p0; // Poles ...
		  Sample f2p1;
		  Sample f2p2;
		  Sample f2p3;

		  // Sample history buffer

		  Sample sdm1; // Sample data minus 1
		  Sample sdm2; // 2
		  Sample sdm3; // 3

					   // Gain Controls

		  Sample lg; // low gain
		  Sample mg; // mid gain
		  Sample hg; // high gain

		  void init(double lowfreq, double mixfreq, double highfreq)
		  {
			  // Set Low/Mid/High gains to unity

			  this->lg = Sample(1.0);
			  this->mg = Sample(1.0);
			  this->hg = Sample(1.0);

			  // Calculate filter cutoff frequencies

			  this->lf = Sample(2 * sin(3.14159265359 * ((double)lowfreq / (double)mixfreq)));
			  this->hf = Sample(2 * sin(3.14159265359 * ((double)highfreq / (double)mixfreq)));
			}

			Sample doThreeBand(Sample const& sample)
			{
				// Locals
				const Sample vsa = Sample(1.0 / 4294967295.0); // Very small amount (Denormal Fix)

				Sample l, m, h; // Low / Mid / High - Sample Values

								// Filter #1 (lowpass)

				this->f1p0 += (this->lf * (sample - this->f1p0)) + vsa;
				this->f1p1 += (this->lf * (this->f1p0 - this->f1p1));
				this->f1p2 += (this->lf * (this->f1p1 - this->f1p2));
				this->f1p3 += (this->lf * (this->f1p2 - this->f1p3));

				l = this->f1p3;

				// Filter #2 (highpass)

				this->f2p0 += (this->hf * (sample - this->f2p0)) + vsa;
				this->f2p1 += (this->hf * (this->f2p0 - this->f2p1));
				this->f2p2 += (this->hf * (this->f2p1 - this->f2p2));
				this->f2p3 += (this->hf * (this->f2p2 - this->f2p3));

				h = this->sdm3 - this->f2p3;

				// Calculate midrange (signal - (low + high))

				m = this->sdm3 - (h + l);

				// Scale, Combine and store

				l *= this->lg;
				m *= this->mg;
				h *= this->hg;

				// Shuffle history buffer

				this->sdm3 = this->sdm2;
				this->sdm2 = this->sdm1;
				this->sdm1 = sample;

				// Return result

				return(l + m + h);
			  }

		} Sample_EQSTATE;

		void processEqualizer(Sample* data, size_t numberOfSamples, Sample_EQSTATE* es,
			float low, float mid, float high, float lowfreq, float highfreq);
	}

	namespace distortion
	{
		void processNonlinDistortion(Sample *data, size_t numberOfSamples, size_t algorithm, float inGain, size_t iterations);
	}
}