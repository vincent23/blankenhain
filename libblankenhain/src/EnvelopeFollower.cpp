#include "EnvelopeFollower.h"
#include "constants.h"
#include "BhMath.h"

EnvelopeFollower::EnvelopeFollower()
	: rmsWindow(128u)
{}

EnvelopeFollower::EnvelopeFollower(unsigned int bufferSize)
	: rmsWindow(bufferSize)
{}

void EnvelopeFollower::setTimes(float attack, float release) {
	// times are in ms
	float msPerSamples = 1000.f / constants::sampleRate;
	attackGain = BhMath::exp(-1.f / attack * msPerSamples);
	releaseGain = BhMath::exp(-1.f / release * msPerSamples);
}

Sample _vectorcall EnvelopeFollower::nextPeakEnvelope(Sample in)
{
	return nextEnvelope(getPeakSample(in));
}

Sample _vectorcall EnvelopeFollower::nextRmsEnvelope(Sample in)
{
	return nextEnvelope(getRmsSample(in));
}

Sample _vectorcall EnvelopeFollower::getCurrentEnvelope() const
{
	return envelope;
}

Sample _vectorcall EnvelopeFollower::nextEnvelope(Sample envelopeSample)
{
	// TODO do this in a fancy simd way without unpacking
	alignas(16) floatType envelopeUnpacked[2];
	alignas(16) floatType envelopeSampleUnpacked[2];
	envelope.store_aligned(envelopeUnpacked);
	envelopeSample.store_aligned(envelopeSampleUnpacked);
	alignas(16) floatType currentGainUnpacked[2];
	for (unsigned int i = 0; i < 2; i++) {
		currentGainUnpacked[i] = envelopeUnpacked[i] < envelopeSampleUnpacked[i] ? attackGain : releaseGain;
	}
	Sample currentGain(currentGainUnpacked[0], currentGainUnpacked[1]);
	envelope = envelopeSample + currentGain * (envelope - envelopeSample);
	return envelope;
}

Sample _vectorcall EnvelopeFollower::getPeakSample(Sample in)
{
	return in.abs();
}

Sample _vectorcall EnvelopeFollower::getRmsSample(Sample in)
{
	Sample inSquared = in * in;
	total += inSquared - rmsWindow.get(127u);
	rmsWindow.push(inSquared);
	return (total / Sample(128.f)).sqrt();
}