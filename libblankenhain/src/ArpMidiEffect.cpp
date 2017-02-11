#include "ArpMidiEffect.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"

ArpMidiEffect::ArpMidiEffect()
	: MidiBase(1, true)
{
	ParameterBundle* params = getPointerToParameterBundle();

	params->getParameter(0) = new FloatParameter(0.f, NormalizedRange(-12.f, 12.f), "shift", "semitones");
}

void ArpMidiEffect::processMidiEvents(MidiEvent* events, unsigned int& numberOfMidiEvents, unsigned int maxNumEvents, unsigned int numberOfSamples)
{
	// TODO
	// for now, ignore incoming midi note and pretend c-e-g is held

	// so first, we have to clear the notes
	numberOfMidiEvents = 0;

	float beatPosition = aux::positionToTime(tempodata.position) * (tempodata.bpm / 60.f);

	// TODO well, actually this does nothing right now
}
