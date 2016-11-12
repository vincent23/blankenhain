#pragma once

#ifdef _BH2_pan

#include "bh2_base.h"


class bh2_pan_effect : public BH2_effect_base
{
public:
  bh2_pan_effect() : BH2_effect_base(128u)
  {
    this->params = new ParameterBundle(2);
    (params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(-1.f, 1.f),  "pan", "");
    (params->getParameter(1)) = new ParameterWithProperties(0.f, NormalizedRange(), "mono", "bool");
  }

  ~bh2_pan_effect()
  {
    for (size_t i = 0u; i < this->params->getNumberOfParameters(); i++)
      delete params->getParameter(i);
    delete params;
  }

  void processFunction(size_t sampleFrames)
  {
    bool mono = this->params->getParameter(1)->getCurrentValueNormalized() > 0.5 ? true : false;
    float panningValue = this->params->getParameter(0)->getCurrentValueNormalized();
    if (mono)
    {
      for (size_t bufferIteration = 0u; bufferIteration < sampleFrames; bufferIteration++)
      {
        alignas(16) double lr[2];
        sseBuffer[bufferIteration].store_aligned(lr);
        lr[1] = lr[0] * (1.f + (0.0f < panningValue ? 0.0f : panningValue) * 0.02f);
        lr[0] = lr[0] * (1.f - (0.0f < panningValue ? panningValue : 0.0f) * 0.02f);
        sseBuffer[bufferIteration] = load_aligned(lr);
      }
    }
    else
    {
      for (size_t bufferIteration = 0u; bufferIteration < sampleFrames; bufferIteration++)
      {
        alignas(16) double lr[2];
        sseBuffer[bufferIteration].store_aligned(lr);
        lr[0] = lr[0] * (1.f - (0.0f < panningValue ? panningValue : 0.0f) * 0.02f);
        lr[1] = lr[1] * (1.f + (0.0f < panningValue ? 0.0f : panningValue) * 0.02f);
        sseBuffer[bufferIteration] = load_aligned(lr);
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
    delete bh_base;
    delete vstparameters;
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