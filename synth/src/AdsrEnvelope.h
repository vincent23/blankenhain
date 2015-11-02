#ifndef ADSR_ENVELOPE_H_INCLUDED
#define ADSR_ENVELOPE_H_INCLUDED

namespace AdsrEnvelope {
	double value(double timeSinceNoteOn, double timeSinceNoteOff, double attack, double decay, double sustain, double release);
}

#endif // ADSR_ENVELOPE_H_INCLUDED