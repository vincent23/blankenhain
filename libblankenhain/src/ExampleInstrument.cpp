#include "ExampleInstrument.h"

#include "ParameterBundle.h"
#include "FloatParameter.h"

ExampleInstrument::ExampleInstrument()
	: InstrumentBase(1)
{
	getPointerToParameterBundle()->getParameter(0) = new FloatParameter(1.f, NormalizedRange(0.01f, 12.f, 1.f), "ratio", "");
}


void ExampleInstrument::process(Sample* buffer, unsigned int numberOfSamples)
{
	nextSample(numberOfSamples);
}