#include "pitchshiftMidiEffect.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"

pitchshiftMidiEffect::pitchshiftMidiEffect()
	: MidiBase(1), oldShift(0)
{
	ParameterBundle& params = getParameterBundle();

	for (unsigned int i = 0u; i < 128u; i++)
		whichNotesAreOn[i] = false;

	params.getParameter(0) = new FloatParameter(0.f, NormalizedRange(-12.f, 12.f), "shift", "semitones");
}

void pitchshiftMidiEffect::processMidiEvents(MidiEvent* events, unsigned int& numberOfMidiEvents, unsigned int maxNumEvents, unsigned int numberOfSamples)
{
	// calculate new shift
	int shift = static_cast<int>(interpolatedParameters.get(0));

	// Send noteOff for all notes still on when pitch changes
	// otherwise they WILL NEVER STOP SOUNDING AND IT IS HORRIBLE!!!
	if (oldShift != shift && numberOfMidiEvents == 0u)
	{
		unsigned int count = 0u;
		for (unsigned int i = 0u; i < 128u; i++)
		{
			if (whichNotesAreOn[i])
			{
				events[count] = MidiEvent(0u, i, 0u);
				whichNotesAreOn[i] = false;
				count++;
			}
		}
		numberOfMidiEvents = count;
		oldShift = shift;
	}
	else
	{
		for (unsigned int i = 0u; i < numberOfMidiEvents; i++)
		{
			int newNote = static_cast<int>(events[i].key) + shift;
			if (newNote < 127 && newNote > 0u)
				events[i].key = static_cast<unsigned int>(newNote);
			else if (newNote < 0)
				events[i].key = 0u;
			else
				events[i].key = 127u;
			if (events[i].velocity == 0)
				whichNotesAreOn[events[i].key] = false;
			else
				whichNotesAreOn[events[i].key] = true;
		}
	}
}
