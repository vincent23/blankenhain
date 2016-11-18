
//This is an example for a bh2 plugin
#pragma once

// Here, enter the number of GUI parameters you want to have
#define NUMBER_OF_PARAMETERS 3

// Don't change this
#include "bh2_base.h"

// Include additional headers here
#include <algorithm>
//

// Here, change the name
class bh2_filter_effect : public BH2_effect_base
{
  // Include optional additional stuff
protected:
  effects::filter::Filter filter;

public:
  // Change the name and define parameters in constructor
  bh2_filter_effect() : BH2_effect_base(512u)
  {
    // Don't change this ////////////////////////////////////////////
    this->currentParameters = new float[NUMBER_OF_PARAMETERS];   ////
    this->params = new ParameterBundle(NUMBER_OF_PARAMETERS);    ////
    /////////////////////////////////////////////////////////////////

    // Insert your stuff here
    (params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(0.00f, 5.f, 1.f),  "resonance", "");
    (params->getParameter(1)) = new ParameterWithProperties(120.f, NormalizedRange(40.f, 22000.f, 0.3f), "frequency", "hz");
    (params->getParameter(2)) = new ParameterWithProperties(0.f, NormalizedRange(0.f, 1.f, 1.f), "style", "");


    //resonance = new FloatParameter(0.5, "Resonance", 0.5f, NormalizedRange(0.f, 5.f));
    //frequency = new FloatParameter((500.f), "Frequency", 0.5f, NormalizedRange(40.f, 22000.f, 0.3f));
  }

  // Change the title
  // Don't change the code, unless you have malloced something and need to construct it
  ~bh2_filter_effect()
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

  /* Here, you will perform the processing of the Buffer
  * @param buffer: Samples to be processed
  * @param numberOfSamples: Size of the buffer
  * @param parameters: Parameters for the effect, defined in constructor
  * @param numberOfParameters: Number of Parameters
  */
  void process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters)
  {
    float& resonance = parameters[0];
    float& frequency = parameters[1];
    float& style  = parameters[2];
    if (style < 0.25)
        filter.processHigh(buffer, numberOfSamples, frequency, resonance);
    else if (style < 0.5)
        filter.processLow(buffer, numberOfSamples, frequency, resonance);
    else if (style < 0.75)
        filter.processBand(buffer, numberOfSamples, frequency, resonance);
    else
       filter.processNotch(buffer, numberOfSamples, frequency, resonance);
  }


};

// Change the name here
class BH2_filter : public BH2_base
{
public:
  BH2_filter(audioMasterCallback audioMaster) :
    BH2_base(audioMaster, NUMBER_OF_PARAMETERS)
  {
    bh_base = new bh2_filter_effect();
    vstparameters = new VSTParameterBundle(NUMBER_OF_PARAMETERS, bh_base->getPointerToParameterBundle());
  }

  ~BH2_filter()
  {
    if (bh_base != nullptr) delete bh_base;
    bh_base = nullptr;
    if (vstparameters != nullptr) delete vstparameters;
    vstparameters = nullptr;
  }

  virtual void open()
  {
    setUniqueID(1993);
    isSynth(false);
  }

  virtual void close()
  {

  }
};