#pragma once

namespace AdsrEnvelope {
	double value(double timeSinceNoteOn, double timeSinceNoteOff, double attack, double decay, double sustain, double release);
}