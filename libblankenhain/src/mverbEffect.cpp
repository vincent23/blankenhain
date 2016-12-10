#include "mverbEffect.h"

#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "InterpolatedValue.h"

#include <algorithm>

// Here, enter the number of GUI parameters you want to have
const unsigned int NUMBER_OF_PARAMETERS = 6u;

// Change the name and define parameters in constructor
mverbEffect::mverbEffect() : EffectBase(NUMBER_OF_PARAMETERS)
{
	ParameterBundle* params = getPointerToParameterBundle();


	// Insert your stuff here
	(params->getParameter(0)) = new FloatParameter(0.75f, NormalizedRange(), "roomSize", "");
	(params->getParameter(1)) = new FloatParameter(0.25f, NormalizedRange(), "damping", "");
	(params->getParameter(2)) = new FloatParameter(0.f, NormalizedRange(), "width", "");
	(params->getParameter(3)) = new FloatParameter(0.f, NormalizedRange(true), "mode", "bool");
	(params->getParameter(4)) = new FloatParameter(-6.f, NormalizedRange(-120.f, 0.f, 5.f), "dry", "dB");
  (params->getParameter(5)) = new FloatParameter(-60.f, NormalizedRange(-120.f, 0.f, 5.f), "wet", "dB");

  ///////////////////////
  // More stuff //         

  combDelay_ = new CircularBuffer<Sample>*[nCombs];
  allPassDelay_ = new CircularBuffer<Sample>*[nAllpasses];
  combLP_ = new OnePoleFilter<Sample>*[nCombs];


  for (size_t i = 0u; i < this->nCombs; i++)
  {
    combDelay_[i] = new CircularBuffer<Sample>(cDelayLengths[i]);
    combLP_[i] = new OnePoleFilter<Sample>(Sample(0.5));
  }
  for (size_t i = 0u; i < this->nAllpasses; i++)
  {
    allPassDelay_[i] = new CircularBuffer<Sample>(aDelayLengths[i]);
  }

}

mverbEffect::~mverbEffect()
{
  for (size_t i = 0u; i < this->nCombs; i++)
  {
    delete combDelay_[i];
    delete combLP_[i];
  }
  for (size_t i = 0u; i < this->nAllpasses; i++)
  {
    delete allPassDelay_[i];
  }
  delete[] combDelay_;
  delete[] allPassDelay_;
  delete[] combLP_;

}


void mverbEffect::process(Sample* buffer, size_t numberOfSamples)
{
  InterpolatedValue& roomSize = getParameterValue(0);



  InterpolatedValue& width = getParameterValue(2);

  const bool mode = getParameterValue(3).get() > 0.5 ? true : false;

  float damping = getParameterValue(1).get();
  damping = mode ? 0.f : damping * scaleDamp;

  InterpolatedValue& dry = getParameterValue(4);

  InterpolatedValue& wet = getParameterValue(5);


  // set low pass filter for delay output
  for (int i = 0; i < nCombs; i++) {
    combLP_[i]->setParams(Sample(1.0 - damping), Sample(-1.0 * damping));
  }

	for (size_t i = 0; i < numberOfSamples; i++)
	{
    const float currentRoomSize = mode ? 1.f : (roomSize.get() * scaleRoom) + offsetRoom;


    Sample fInput = buffer[i];
    Sample out(0.0);

    // Parallel LBCF filters
    for (int j = 0; j < nCombs; j++) {
      // Left channel
      Sample yn = fInput + (Sample(currentRoomSize) * combLP_[j]->tick(combDelay_[j]->get()));
      combDelay_[j]->pushpop(yn);
      out += yn;

    }

    // Series allpass filters
    for (int j = 0; j < nAllpasses; j++) {
      // Left channel
      Sample vn_m = allPassDelay_[j]->get();
      Sample vn = out + (Sample(g_) * vn_m);
      allPassDelay_[j]->pushpop(vn);

      // calculate output
      out = Sample(-1.) * vn + Sample((1.0 + g_))*vn_m;

    }

    // Mix output
    out = out * aux::decibelToLinear(wet.get()) + buffer[i] * aux::decibelToLinear(dry.get());

    buffer[i] = out;
    nextSample();
	}
	

}