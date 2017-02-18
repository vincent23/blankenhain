#pragma once

#include "Sample.h"
#include "CircularBuffer.h"

/**
 * Enevolpe Follower keeps track of the RMS as well as peak envelope
 * It contains a circular buffer of a certain size and can yield root-mean-square
 * as well as peak values of the data stored within. 
 *
 * Usage:
 * Peak: Currently this is a dummy, only returning back the abs of your input sample. We do not keep track of peaking values yet
 *
 * RMS: Pass in a current value and get back the RMS value. Depends on buffer size.
 *
 * Envelope: Uses attack and release time set by setTimes function to adjust the envelope. Returns adjusted envelope.
 */
class EnvelopeFollower {
public:
	EnvelopeFollower();
	EnvelopeFollower(unsigned int bufferSize);
	void setTimes(float attack, float release);
	Sample _vectorcall nextPeakEnvelope(const Sample& in);
	Sample _vectorcall nextRmsEnvelope(const Sample& in);
	Sample _vectorcall getCurrentEnvelope() const;

private:
	Sample _vectorcall nextEnvelope(const Sample& envelopeSample);
	Sample _vectorcall getPeakSample(const Sample& in);
	Sample _vectorcall getRmsSample(const Sample& in);

	// Stuff for rmsd envelope
	CircularBuffer<Sample> rmsWindow;
	Sample total;

	//CircularBuffer<Sample> peakValue;
	Sample envelope;

	double attackGain;
	double releaseGain;
};