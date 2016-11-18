#pragma once
//This is an example for a bh2 plugin

// Here, enter the number of GUI parameters you want to have
#define NUMBER_OF_PARAMETERS 5

// Don't change this
#include "bh2_base.h"

// Include additional headers here
#include "CircularBuffer.h"
#include <algorithm>
//

// Here, change the name
class bh2_example_effect : public BH2_effect_base
{
	// Include optional additional stuff
protected:
	CircularBuffer<Sample> delayLine;
	Sample envelope;
public:
	// Change the name and define parameters in constructor
	bh2_example_effect() : BH2_effect_base(512u), delayLine(aux::millisecToSamples(2502u))
	{
		// Don't change this //////////////////////////////////////////
		this->currentParameters = new float[NUMBER_OF_PARAMETERS];   //
		this->params = new ParameterBundle(NUMBER_OF_PARAMETERS);    //
		///////////////////////////////////////////////////////////////

		// Insert your stuff here
		(params->getParameter(0)) = new ParameterWithProperties(1.f, NormalizedRange(0.01f, 12.f, 1.f), "ratio", "");
		(params->getParameter(1)) = new ParameterWithProperties(10.f, NormalizedRange(0.2f, 15.f, 0.5f), "release", "ms");
		(params->getParameter(2)) = new ParameterWithProperties(0.f, NormalizedRange(-48.f, 0.f, 4.5f), "threshold", "dB");
		(params->getParameter(3)) = new ParameterWithProperties(10.f, NormalizedRange(0.2f, 35.f, 1.f), "attack", "ms");
		(params->getParameter(4)) = new ParameterWithProperties(0.f, NormalizedRange(0.f, 100.f, 1.f), "limiterOn", "");

	}

	// Change the title
	// Don't change the code, unless you have malloced something and need to construct it
	~bh2_example_effect()
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
		float& ratio = parameters[0];
		float& release = parameters[1];
		float& threshold = parameters[2];
		float& attack = parameters[3];
		bool limiterIsOn = parameters[4] > 0.5 ? true : false;
		size_t attackTimeInSamples = aux::millisecToSamples(attack);
		delayLine.setSize(attackTimeInSamples);
		for (size_t i = 0; i < numberOfSamples; i++)
		{
			buffer[i] = delayLine.pushpop(buffer[i]);
		}
	}


};

// Change the name here
class BH2_example : public BH2_base
{
public:
	BH2_example(audioMasterCallback audioMaster) :
		BH2_base(audioMaster, NUMBER_OF_PARAMETERS)
	{
		bh_base = new bh2_example_effect();
		vstparameters = new VSTParameterBundle(NUMBER_OF_PARAMETERS, bh_base->getPointerToParameterBundle());
	}

	~BH2_example()
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