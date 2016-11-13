#pragma once

#ifdef _BH2_equalizer

#include "bh2_base.h"
#include <iostream>



class bh2_equalizer_effect : public BH2_effect_base
{
private:
  effects::equalizer::Sample_EQSTATE* es;
  const double vsa = (1.0 / 4294967295.0); // Very small amount (Denormal Fix)
public:
  bh2_equalizer_effect() : BH2_effect_base(512)
  {
    this->currentParameters = new float[5];
    this->params = new ParameterBundle(5);
    (params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(-36.f, 12.f, 1.f),  "low", "dB");
    (params->getParameter(1)) = new ParameterWithProperties(0.f, NormalizedRange(-36.f, 12.f, 1.f), "mid", "dB");
    (params->getParameter(2)) = new ParameterWithProperties(0.f, NormalizedRange(-36.f, 12.f, 1.f), "high", "dB");
    (params->getParameter(3)) = new ParameterWithProperties(530.f, NormalizedRange(40.f, 20000.f),  "LowFreq", "1/s");
    (params->getParameter(4)) = new ParameterWithProperties(5700.f, NormalizedRange(40.f, 20000.f), "HighFreq", "1/s");


    for (unsigned int i = 0u; i < this->params->getNumberOfParameters(); i++)
      this->currentParameters[i] = this->params->getParameter(i)->getDefaultValueUnnormalized();

    es = new effects::equalizer::Sample_EQSTATE();
    es->init(100., 44100., 5000.);

  }

  ~bh2_equalizer_effect()
  {
    delete es;

    for (size_t i = 0u; i < this->params->getNumberOfParameters(); i++) {
      if (params->getParameter(i) != nullptr) delete params->getParameter(i);
      params->getParameter(i) = nullptr;
    }
    if (params != nullptr) delete params;
    params = nullptr;
    if (currentParameters != nullptr) delete[] currentParameters;
    currentParameters = nullptr;
  }

  void process(Sample* buffer, size_t sampleFrames, size_t numberOfParameters, float* parameters)
  {
    float& low = parameters[0];
    float& mid= parameters[1];
    float& high = parameters[2];
    float& lowfreq = parameters[3];
    float& highfreq = parameters[4];


    const double mixfreq = 44100;
    es->lf = Sample(2 * sin(3.14159265359 * ((double)lowfreq / mixfreq)));
    es->hf = Sample(2 * sin(3.14159265359 * ((double)highfreq / mixfreq)));
    es->lg = Sample(aux::decibelToLinear(low));
    es->mg = Sample(aux::decibelToLinear(mid));
    es->hg = Sample(aux::decibelToLinear(high));

    for (size_t i = 0; i < sampleFrames; i++)
    {
      buffer[i] = es->doThreeBand(buffer[i]);
    }

  }

};


class BH2_equalizer : public BH2_base
{
public:
  BH2_equalizer(audioMasterCallback audioMaster) :
    BH2_base(audioMaster, 5) 
  {
    bh_base = new bh2_equalizer_effect();
    vstparameters = new VSTParameterBundle(5u, bh_base->getPointerToParameterBundle());
  }

  ~BH2_equalizer()
  {
    if (bh_base != nullptr) delete bh_base;
    bh_base = nullptr;
    if (vstparameters != nullptr) delete vstparameters;
    vstparameters = nullptr;
  }

  virtual void open()
  {
    setUniqueID(189);
    isSynth(false);
  }

  virtual void close()
  {

  }
};

#endif