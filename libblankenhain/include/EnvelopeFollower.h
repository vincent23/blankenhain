#pragma once

#include "Sample.h"
#include "CircularBuffer.h"

class EnvelopeFollower {
public:
	EnvelopeFollower();
	void setTimes(double attack, double release);
	Sample getPeakEnvelope(const Sample& in);
	Sample getRmsEnvelope(const Sample& in);
	Sample getCurrentEnvelope();

private:
	Sample getEnvelope(const Sample& envelopeSample);
	Sample getPeakSample(const Sample& in);
	Sample getRmsSample(const Sample& in);

	CircularBuffer<Sample> rmsWindow;
	Sample total;
	Sample envelope;
	double attackGain;
	double releaseGain;
};