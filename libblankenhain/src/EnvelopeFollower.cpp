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

Sample EnvelopeFollower::nextPeakEnvelope(const Sample& in)
{
	return nextEnvelope(getPeakSample(in));
}

Sample EnvelopeFollower::nextRmsEnvelope(const Sample& in)
{
	return nextEnvelope(getRmsSample(in));
}

Sample EnvelopeFollower::getCurrentEnvelope() const
{
	return envelope;
}

Sample EnvelopeFollower::nextEnvelope(const Sample& envelopeSample)
{
	// TODO do this in a fancy simd way without unpacking
	alignas(16) double envelopeUnpacked[2];
	alignas(16) double envelopeSampleUnpacked[2];
	envelope.store_aligned(envelopeUnpacked);
	envelopeSample.store_aligned(envelopeSampleUnpacked);
	alignas(16) double currentGainUnpacked[2];
	for (unsigned int i = 0; i < 2; i++) {
		currentGainUnpacked[i] = envelopeUnpacked[i] < envelopeSampleUnpacked[i] ? attackGain : releaseGain;
	}
	Sample currentGain = Sample::load_aligned(currentGainUnpacked);
	envelope = envelopeSample + currentGain * (envelope - envelopeSample);
	return envelope;
}

Sample EnvelopeFollower::getPeakSample(const Sample& in)
{
	return in.abs();
}

Sample EnvelopeFollower::getRmsSample(const Sample& in)
{
	Sample inSquared = in * in;
	total += inSquared - rmsWindow.pushpop(inSquared);
	return (total / Sample(static_cast<double>(rmsWindow.getSize()))).sqrt();
}