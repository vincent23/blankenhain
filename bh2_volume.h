#pragma once
#include "bh2_base.h"

class bh2_volume_effect : public BH2_effect_base
{
public:
  bh2_volume_effect()
  {
    this->params = new ParameterBundle(3);
    (params->getParameter(0)) = new ParameterWithProperties(120.f / 132.f, NormalizedRange(-120.f, 12.f, 5.f),  "volumeL", "dB");
    (params->getParameter(1)) = new ParameterWithProperties(120.f / 132.f, NormalizedRange(-120.f, 12.f, 5.f),  "volumeR", "dB");
    (params->getParameter(2)) = new ParameterWithProperties(0.f, NormalizedRange(), "coupling", "bool");
  }

  ~bh2_volume_effect()
  {
    for (size_t i = 0u; i < this->params->getNumberOfParameters(); i++)
      delete params->getParameter(i);
    delete params;
  }

  void process(float** inputs, float** outputs, unsigned int sampleFrames)
  {


    //float& currentVolumeL, currentVolumeR, bufferValue, oldVolumeL, oldVolumeR;
    float currentVolumeL = this->params->getParameter(0)->getCurrentValueNormalized();
    float currentVolumeR = this->params->getParameter(1)->getCurrentValueNormalized();
    float oldVolumeL = this->params->getParameter(0)->getOldValueNormalized();
    float oldVolumeR = this->params->getParameter(1)->getOldValueNormalized();
    bool coupling = this->params->getParameter(2)->getCurrentValueNormalized() > 0.5 ? true : false;

    size_t maxInterpolation;

    float *in_l = inputs[0];
    float *in_r = inputs[1];
    float *out_l = outputs[0];
    float *out_r = outputs[1];

    if (coupling)
    {
      // Interpolation

      maxInterpolation = static_cast<size_t>(sampleFrames * this->bufferScalingValue);
      for (size_t interpolationIteration = 0; interpolationIteration < maxInterpolation; interpolationIteration++)
      {

        out_l[interpolationIteration] = in_l[interpolationIteration] * \
          (oldVolumeL + ((interpolationIteration + 1) * (currentVolumeL - oldVolumeL) \
            / maxInterpolation));

        out_r[interpolationIteration] = in_r[interpolationIteration] * \
          (oldVolumeL + ((interpolationIteration + 1) * (currentVolumeL - oldVolumeL) \
            / maxInterpolation));

      }
      for (
        size_t bufferIteration = maxInterpolation;
        static_cast<int>(bufferIteration) < sampleFrames;
        bufferIteration++
        )
      {
        out_l[bufferIteration] = in_l[bufferIteration] * currentVolumeL;
        out_r[bufferIteration] = in_r[bufferIteration] * currentVolumeL;
      }
    }
    else
    {
      // Interpolation

      maxInterpolation = static_cast<size_t>(sampleFrames * this->bufferScalingValue);
      for (size_t interpolationIteration = 0; interpolationIteration < maxInterpolation; interpolationIteration++)
      {

        out_l[interpolationIteration] = in_l[interpolationIteration] * \
          (oldVolumeL + ((interpolationIteration + 1) * (currentVolumeL - oldVolumeL) \
            / maxInterpolation));

        out_r[interpolationIteration] = in_r[interpolationIteration] * \
          (oldVolumeR + ((interpolationIteration + 1) * (currentVolumeR - oldVolumeR) \
            / maxInterpolation));

      }
      for (
        size_t bufferIteration = maxInterpolation;
        static_cast<int>(bufferIteration) < sampleFrames;
        bufferIteration++
        )
      {
        out_l[bufferIteration] = in_l[bufferIteration] * currentVolumeL;
        out_r[bufferIteration] = in_r[bufferIteration] * currentVolumeR;
      }
    }
    //Set current values as old values for interpolation in next buffer iteration
    this->params->getParameter(0u)->setOldValueUnnormalized(this->params->getParameter(0u)->getCurrentValueUnnormalized());
    this->params->getParameter(1u)->setOldValueUnnormalized(this->params->getParameter(1u)->getCurrentValueUnnormalized());

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
    delete bh_base;
    delete vstparameters;
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