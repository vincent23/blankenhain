#pragma once

#ifdef _BH2_delay

#include "bh2_base.h"
#include "CircularBuffer.h"
#include <algorithm>


class bh2_delay_effect : public BH2_effect_base
{
public:
  bh2_delay_effect() : BH2_effect_base(512u), delayLine(aux::millisecToSamples(2502u))
  {
    this->currentParameters = new float[4];
    this->params = new ParameterBundle(4);
    (params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(-50.f, 50.f),  "pan", "");
    (params->getParameter(1)) = new ParameterWithProperties(100.f, NormalizedRange(1.f, 2500.f), "length", "ms");
    (params->getParameter(2)) = new ParameterWithProperties(0.f, NormalizedRange(0.f, 1.5f), "feedback", "");
    (params->getParameter(3)) = new ParameterWithProperties(1.f, NormalizedRange(), "drywet", "");
  }

  ~bh2_delay_effect()
  {
    for (size_t i = 0u; i < this->params->getNumberOfParameters(); i++) {
      if (params->getParameter(i) != nullptr) delete params->getParameter(i);
      params->getParameter(i) = nullptr;
    }
    if (params != nullptr) delete params;
    params = nullptr;
    if (currentParameters != nullptr) delete[] currentParameters;
    currentParameters = nullptr;
  }

  void process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters)
  {
    float& pan = parameters[0];
    float& length = parameters[1];
    float& feedback = parameters[2];
    float& drywet = parameters[3];

    delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(length)));
    for (size_t i = 0; i < numberOfSamples; i++)
    {
      double avg_ = avgValue(buffer[i]);
      Sample original = buffer[i];
      Sample line = delayLine.get();

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

      buffer[i] = original + line;

      delayLine.push(delayLine.get() * Sample(feedback) + Sample(avg_));
    }
  }
protected:
  CircularBuffer<Sample> delayLine;

};


class BH2_delay : public BH2_base
{
public:
  BH2_delay(audioMasterCallback audioMaster) :
    BH2_base(audioMaster, 4) 
  {
    bh_base = new bh2_delay_effect();
    vstparameters = new VSTParameterBundle(4u, bh_base->getPointerToParameterBundle());
  }

  ~BH2_delay()
  {
    if (bh_base != nullptr) delete bh_base;
    bh_base = nullptr;
    if (vstparameters != nullptr) delete vstparameters;
    vstparameters = nullptr;
  }

  virtual void open()
  {
    setUniqueID(1991);
    isSynth(false);
  }

  virtual void close()
  {

  }
};

#endif