#pragma once

#ifdef _BH2_distortion

#include "bh2_base.h"
#include <iostream>



class bh2_distortion_effect : public BH2_effect_base
{
public:
  bh2_distortion_effect() : BH2_effect_base(265)
  {
    this->currentParameters = new float[3];
    this->params = new ParameterBundle(3);
    (params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(-12.f, 12.f, 1.f),  "inGain", "dB");
    (params->getParameter(1)) = new ParameterWithProperties(0.f, NormalizedRange(0.f, 10.f, 1.f),  "iterations", "");
    (params->getParameter(2)) = new ParameterWithProperties(1.f, NormalizedRange(), "algorithm", "");
  }

  ~bh2_distortion_effect()
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

  enum distortionAlgorithms
  {
    ArayaAndSuyama = 0, DoidicSymmetric,
    DoidicAsymmetric
  };

  void process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters)
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

};


class BH2_distortion : public BH2_base
{
public:
  BH2_distortion(audioMasterCallback audioMaster) :
    BH2_base(audioMaster, 3) 
  {
    bh_base = new bh2_distortion_effect();
    vstparameters = new VSTParameterBundle(3u, bh_base->getPointerToParameterBundle());
  }

  ~BH2_distortion()
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