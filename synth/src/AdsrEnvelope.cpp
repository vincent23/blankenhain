#include "AdsrEnvelope.h"

double AdsrEnvelope::value(double timeSinceNoteOn, double timeSinceNoteOff, double attack, double decay, double sustain, double release) {
	if (timeSinceNoteOff < 0) {
		if (timeSinceNoteOn < attack) {
			return timeSinceNoteOn / attack;
		}
		else if (timeSinceNoteOn < attack + decay) {
			return 1 - (timeSinceNoteOn - attack) / decay * (1 - sustain);
		}
		else {
			return sustain;
		}
	}
	else {
		if (timeSinceNoteOff < release) {
			return sustain - sustain * timeSinceNoteOff / release;
		}
		else {
			return 0;
		}
	}
}