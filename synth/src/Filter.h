#pragma once

#include "Sample.h"

namespace FilterConstants {
	constexpr size_t blockSize = 16;
}

struct Filter : public AlignedType {
	Filter();

	void processHigh(Sample* data, size_t numberOfSamples, double frequency, double resonance);
	void processLow(Sample* data, size_t numberOfSamples, double frequency, double resonance);
	void processBand(Sample* data, size_t numberOfSamples, double frequency, double resonance);
	void processNotch(Sample* data, size_t numberOfSamples, double frequency, double resonance);
	void process(Sample* data, size_t numberOfSamples, double a1, double a2, double b0, double b1, double b2);

	Sample x_1, y_1, y_2;
};