#pragma once

#include "InstrumentBase.h"

class ExampleInstrument : public InstrumentBase
{
public:
	ExampleInstrument();

	void process(Sample* buffer, unsigned int numberOfSamples) override;
};