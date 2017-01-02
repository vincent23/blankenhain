#include "pitchshiftMidiEffect.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"

pitchshiftMidiEffect::pitchshiftMidiEffect()
	: MidiBase(1)
{
	ParameterBundle* params = getPointerToParameterBundle();

	params->getParameter(0) = new FloatParameter(0.f, NormalizedRange(-12.f, 12.f), "shift", "semitones");
}

void pitchshiftMidiEffect::processMidiEvents(MidiEvent*& events, unsigned int numberOfMidiEvents)
{
	int shift = static_cast<int>(getInterpolatedParameter(0).get());
	for (unsigned int i = 0u; i < numberOfMidiEvents; i++)
	{
		int newNote = static_cast<int>(events[i].key) + shift;
		if (newNote < 127 && newNote > 0u)
			events[i].key = static_cast<unsigned int>(newNote);
		else if (newNote < 0)
			events[i].key = 0u;
		else
			events[i].key = 127u;
	}
}
