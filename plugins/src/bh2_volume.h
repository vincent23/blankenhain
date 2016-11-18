#pragma once

#include "bh2_base.h"
#include <iostream>



class bh2_volume_effect : public BH2_effect_base
{
public:
  bh2_volume_effect() : BH2_effect_base(512)
  {
    this->currentParameters = new float[3];
    this->params = new ParameterBundle(3);
    (params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(-120.f, 12.f, 5.f),  "volumeL", "dB");
    (params->getParameter(1)) = new ParameterWithProperties(0.f, NormalizedRange(-120.f, 12.f, 5.f),  "volumeR", "dB");
    (params->getParameter(2)) = new ParameterWithProperties(1.f, NormalizedRange(), "coupling", "bool");
  }

  ~bh2_volume_effect()
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
    float& currentVolumeL = parameters[0];
    float& currentVolumeR = parameters[1];
    bool coupling = parameters[2] > 0.5 ? true : false;

    alignas(16) double currentBuffer[2];
    if (coupling)
    {
      for (size_t bufferIteration = 0; bufferIteration < sampleFrames; bufferIteration++)
      {
        buffer[bufferIteration] = buffer[bufferIteration] * aux::decibelToLinear(currentVolumeL);
      }
    }
    else
    {
      for (
        size_t bufferIteration = 0u;
        bufferIteration < sampleFrames;
        bufferIteration++
        )
      {
        buffer[bufferIteration].store_aligned(currentBuffer);
        currentBuffer[0] = currentBuffer[0] * aux::decibelToLinear(currentVolumeL);
        currentBuffer[1] = currentBuffer[1] * aux::decibelToLinear(currentVolumeR);
        buffer[bufferIteration].load_aligned(currentBuffer);
      }
    }
  }

};


class BH2_volume : public BH2_base
{
public:
  BH2_volume(audioMasterCallback audioMaster) :
    BH2_base(audioMaster, 3) 
  {
    bh_base = new bh2_volume_effect();
    vstparameters = new VSTParameterBundle(3u, bh_base->getPointerToParameterBundle());
  }

  ~BH2_volume()
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