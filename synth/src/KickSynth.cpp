#include "KickSynth.h"
#include <cmath>

KickSynth::KickSynth()
{
}

void KickSynth::kick()
{
	for (size_t i = 0; i < 3; i++) {
		high[i].reset();
		low[i].reset();
	}
	lowHarmonic.reset();
	samplesSinceKick = 0;
}

float ar(float attack, float release, float t) {
	if (t < attack) {
		return t / attack;
	}
	else if (t < attack + release) {
		return 1. - (t - attack) / release;
	}
	else {
		return 0;
	}
}

void KickSynth::play(Sample* data, size_t numberOfSamples)
{
	const float TAU = 2 * acos(-1.);
	const float B = 750;
	const float wc = .5 * (1000 - 250) + 250;
	const float beta = 5;
	const float wm = .5 * B / (1 + beta);
	const float delta_wc = B * .5 - wm;
	const float attack = .001;
	const float release = .05;
	const float target = 700;
	const float max_f = 4000;
	for (size_t i = 0; i < numberOfSamples; i++) {
		double t = (i + samplesSinceKick) / 44100.;
		data[i] = Sample(cos((wc + delta_wc * cos(wm * t * TAU + .5)) * t * TAU));
		for (size_t j = 0; j < 3; j++) {
			high[j].processHigh(&data[i], 1, 250, 1);
		}
		const float env = ar(attack, release, t);
		const float f = target + (max_f - target) * env;
		for (size_t j = 0; j < 3; j++) {
			low[j].processLow(&data[i], 1, f, .1);
		}

		float env2 = ar(.01, .3, t);
		float pitch = 40 + 15 * env2;
		Sample harmonic = Sample(sin(t * pitch * TAU));
		lowHarmonic.processLow(&harmonic, 1, 50, 1);
		data[i] *= Sample(.3);
		data[i] += harmonic * Sample(.95);
		data[i] *= Sample(env2);
	}
	samplesSinceKick += numberOfSamples;
}
