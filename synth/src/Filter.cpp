#include "Filter.h"
#include <cmath>
#include <algorithm>
Filter::Filter() :
	x_1(0), y_1(0), y_2(0)
{
}

void Filter::processHigh(Sample* data, size_t numberOfSamples, double frequency, double resonance) {
	// TODO sample rate
	const double sampleRate = 44100;
	const double w0 = 2.0 * acos(-1.0) * frequency / (sampleRate * 2);
	const double cos_w0 = cos(w0);
	const double sin_w0 = sin(w0);
	const double Q = std::max(1e-3, resonance);
	const double alpha = sin_w0 / (2 * Q);

	// compute biquad coefficients
	const double a0_inv = 1. / (1. + alpha);
	const double b0 = (1 + cos_w0) / 2 * a0_inv;
	const double b1 = -(1 + cos_w0) * a0_inv;
	const double b2 = (1 + cos_w0) / 2 * a0_inv;
	const double a1 = -2 * cos_w0 * a0_inv;
	const double a2 = (1 - alpha) * a0_inv;
	process(data, numberOfSamples, a1, a2, b0, b1, b2);
}

void Filter::processLow(Sample* data, size_t numberOfSamples, double frequency, double resonance) {
	// TODO sample rate
	const double sampleRate = 44100;
	const double w0 = 2.0 * acos(-1.0) * frequency / (sampleRate * 2);
	const double cos_w0 = cos(w0);
	const double sin_w0 = sin(w0);
	const double Q = std::max(1e-3, resonance);
	const double alpha = sin_w0 / (2 * Q);

	// compute biquad coefficients
	const double a0_inv = 1. / (1. + alpha);
	const double b0 = (1 - cos_w0) / 2 * a0_inv;
	const double b1 = (1 - cos_w0) * a0_inv;
	const double b2 = (1 - cos_w0) / 2 * a0_inv;
	const double a1 = -2 * cos_w0 * a0_inv;
	const double a2 = (1 - alpha) * a0_inv;

	process(data, numberOfSamples, a1, a2, b0, b1, b2);
}

void Filter::processBand(Sample* data, size_t numberOfSamples, double frequency, double resonance) {
	// TODO sample rate
	const double sampleRate = 44100;
	const double w0 = 2.0 * acos(-1.0) * frequency / (sampleRate * 2);
	const double cos_w0 = cos(w0);
	const double sin_w0 = sin(w0);
	const double Q = std::max(1e-3, resonance);
	const double alpha = sin_w0 / (2 * Q);

	// compute biquad coefficients
	// constant 0dB peak gain
	const double a0_inv = 1. / (1. + alpha);
	const double b0 = alpha * a0_inv;
	const double b1 = 0;
	const double b2 = -alpha * a0_inv;
	const double a1 = -2 * cos_w0 * a0_inv;
	const double a2 = (1 - alpha) * a0_inv;
	process(data, numberOfSamples, a1, a2, b0, b1, b2);
}

void Filter::processNotch(Sample* data, size_t numberOfSamples, double frequency, double resonance) {
	// TODO sample rate
	const double sampleRate = 44100;
	const double w0 = 2.0 * acos(-1.0) * frequency / (sampleRate * 2);
	const double cos_w0 = cos(w0);
	const double sin_w0 = sin(w0);
	const double Q = std::max(1e-3, resonance);
	const double alpha = sin_w0 / (2 * Q);

	// compute biquad coefficients
	const double a0_inv = 1. / (1.0 + alpha);
	const double b0 = a0_inv;
	const double b1 = -2 * cos_w0 * a0_inv;
	const double b2 = a0_inv;
	const double a1 = -2 * cos_w0 * a0_inv;
	const double a2 = (1 - alpha) * a0_inv;
	process(data, numberOfSamples, a1, a2, b0, b1, b2);
}

void Filter::process(Sample* data, size_t numberOfSamples, double a1_, double a2_, double b0_, double b1_, double b2_) {
	Sample a1(a1_);
	Sample a2(a2_);
	Sample b0(b0_);
	Sample b1(b1_);
	Sample b2(b2_);

	for (size_t i = 0; i < numberOfSamples; i++) {
		Sample& sample = data[i];

		Sample out = b0 * sample + b1 * x_1 + b2 * x_1 - a1 * y_1 - a2 * y_2;
		Sample out2 = b0 * sample + b1 * sample + b2 * x_1 - a1 * out - a2 * y_1;

		x_1 = sample;
		y_2 = out;
		y_1 = out2;
		sample = out2;
	}
}