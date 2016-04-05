#include "processFunctions.h"
#include "AuxFunc.h"
#include <cmath>
#include <algorithm>

namespace effects
{
  namespace filter
  {
    Filter::Filter()
    {
      reset();
    }

    void Filter::processHigh(Sample* data, size_t numberOfSamples, double frequency, double resonance, double sampleRate) {
      // TODO sample rate
      const double w0 = 2.0 * acos(-1.0) * frequency / (sampleRate * 2);
      const double cos_w0 = cos(w0);
      const double sin_w0 = sin(w0);
      const double Q = std::max(1e-3, resonance);
      const double alpha = sin_w0 / (2 * Q);

      // compute biquad coefficients
      const double a0_inv = 1. / (1. + alpha);
      const double b0 = (1 + cos_w0) / 2 * a0_inv;
      const double b1 = -(1 + cos_w0) * a0_inv;
      const double b2 = (1 + cos_w0) / 2 * a0_inv;
      const double a1 = -2 * cos_w0 * a0_inv;
      const double a2 = (1 - alpha) * a0_inv;
      process(data, numberOfSamples, a1, a2, b0, b1, b2);
    }

    void Filter::processLow(Sample* data, size_t numberOfSamples, double frequency, double resonance, double sampleRate) {
      // TODO sample rate
      const double w0 = 2.0 * acos(-1.0) * frequency / (sampleRate * 2);
      const double cos_w0 = cos(w0);
      const double sin_w0 = sin(w0);
      const double Q = std::max(1e-3, resonance);
      const double alpha = sin_w0 / (2 * Q);

      // compute biquad coefficients
      const double a0_inv = 1. / (1. + alpha);
      const double b0 = (1 - cos_w0) / 2 * a0_inv;
      const double b1 = (1 - cos_w0) * a0_inv;
      const double b2 = (1 - cos_w0) / 2 * a0_inv;
      const double a1 = -2 * cos_w0 * a0_inv;
      const double a2 = (1 - alpha) * a0_inv;

      process(data, numberOfSamples, a1, a2, b0, b1, b2);
    }

    void Filter::processBand(Sample* data, size_t numberOfSamples, double frequency, double resonance, double sampleRate) {
      // TODO sample rate
      const double w0 = 2.0 * acos(-1.0) * frequency / (sampleRate * 2);
      const double cos_w0 = cos(w0);
      const double sin_w0 = sin(w0);
      const double Q = std::max(1e-3, resonance);
      const double alpha = sin_w0 / (2 * Q);

      // compute biquad coefficients
      // constant 0dB peak gain
      const double a0_inv = 1. / (1. + alpha);
      const double b0 = alpha * a0_inv;
      const double b1 = 0;
      const double b2 = -alpha * a0_inv;
      const double a1 = -2 * cos_w0 * a0_inv;
      const double a2 = (1 - alpha) * a0_inv;
      process(data, numberOfSamples, a1, a2, b0, b1, b2);
    }

    void Filter::processNotch(Sample* data, size_t numberOfSamples, double frequency, double resonance, double sampleRate) {
      // TODO sample rate
      const double w0 = 2.0 * acos(-1.0) * frequency / (sampleRate * 2);
      const double cos_w0 = cos(w0);
      const double sin_w0 = sin(w0);
      const double Q = std::max(1e-3, resonance);
      const double alpha = sin_w0 / (2 * Q);

      // compute biquad coefficients
      const double a0_inv = 1. / (1.0 + alpha);
      const double b0 = a0_inv;
      const double b1 = -2 * cos_w0 * a0_inv;
      const double b2 = a0_inv;
      const double a1 = -2 * cos_w0 * a0_inv;
      const double a2 = (1 - alpha) * a0_inv;
      process(data, numberOfSamples, a1, a2, b0, b1, b2);
    }

    void Filter::process(Sample* data, size_t numberOfSamples, double a1_, double a2_, double b0_, double b1_, double b2_) {
      Sample a1(a1_);
      Sample a2(a2_);
      Sample b0(b0_);
      Sample b1(b1_);
      Sample b2(b2_);

      for (size_t i = 0; i < numberOfSamples; i++) {
        Sample& sample = data[i];

        Sample out = b0 * sample + b1 * x_1 + b2 * x_1 - a1 * y_1 - a2 * y_2;
        Sample out2 = b0 * sample + b1 * sample + b2 * x_1 - a1 * out - a2 * y_1;

        x_1 = sample;
        y_2 = out;
        y_1 = out2;
        sample = out2;
      }
    }

    void Filter::reset()
    {
      x_1 = Sample(0);
      y_1 = Sample(0);
      y_2 = Sample(0);
    }
  }

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

  namespace equalizer
  {
    void processEqualizer(Sample* data, size_t numberOfSamples, Sample_EQSTATE* es,
      float low, float mid, float high, float lowfreq, float highfreq)
    {
      const double mixfreq = 44100;
      es->lf = Sample(2 * sin(3.14159265359 * ((double)lowfreq / mixfreq)));
      es->hf = Sample(2 * sin(3.14159265359 * ((double)highfreq / mixfreq)));
      es->lg = Sample(aux::decibelToLinear(low));
      es->mg = Sample(aux::decibelToLinear(mid));
      es->hg = Sample(aux::decibelToLinear(high));

      for (size_t i = 0; i < numberOfSamples; i++)
      {
        data[i] = es->doThreeBand(data[i]);
      }
    }
  }
}