#pragma once

#include "bh2_base.h"


class bh2_pan_effect : public BH2_effect_base
{
public:
  bh2_pan_effect() : BH2_effect_base(512u)
  {
    this->currentParameters = new float[2];
    this->params = new ParameterBundle(2);
    (params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(-50.f, 50.f),  "pan", "");
    (params->getParameter(1)) = new ParameterWithProperties(0.f, NormalizedRange(), "mono", "bool");
  }

  ~bh2_pan_effect()
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

  void process(Sample* buffer, size_t sampleFrames, size_t numberOfParameters, float* parameters)
  {
    float& panningValue = parameters[0];
    bool mono = parameters[1] > 0.5 ? true : false;

    if (mono)
    {
      for (size_t bufferIteration = 0u; bufferIteration < sampleFrames; bufferIteration++)
      {
        alignas(16) double lr[2];
        buffer[bufferIteration].store_aligned(lr);
        lr[1] = lr[0] * (1.f + (0.0f < panningValue ? 0.0f : panningValue) * 0.02f);
        lr[0] = lr[0] * (1.f - (0.0f < panningValue ? panningValue : 0.0f) * 0.02f);
        buffer[bufferIteration] = load_aligned(lr);
      }
    }
    else
    {
      for (size_t bufferIteration = 0u; bufferIteration < sampleFrames; bufferIteration++)
      {
        alignas(16) double lr[2];
        buffer[bufferIteration].store_aligned(lr);
        lr[0] = lr[0] * (1.f - (0.0f < panningValue ? panningValue : 0.0f) * 0.02f);
        lr[1] = lr[1] * (1.f + (0.0f < panningValue ? 0.0f : panningValue) * 0.02f);
        buffer[bufferIteration] = load_aligned(lr);
      }
    }
  }

};


class BH2_pan : public BH2_base
{
public:
  BH2_pan(audioMasterCallback audioMaster) :
    BH2_base(audioMaster, 2) 
  {
    bh_base = new bh2_pan_effect();
    vstparameters = new VSTParameterBundle(2u, bh_base->getPointerToParameterBundle());
  }

  ~BH2_pan()
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