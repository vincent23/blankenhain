#include "EnvelopeFollower.h"
#include "constants.h"
#include "BhMath.h"

EnvelopeFollower::EnvelopeFollower()
	: rmsWindow(128u)
{}

EnvelopeFollower::EnvelopeFollower(unsigned int bufferSize)
	: rmsWindow(bufferSize)
{}

void EnvelopeFollower::setTimes(double attack, double release) {
	// times are in ms
	attack /= 1000.;
	release /= 1000.;
	attackGain = BhMath::exp(-1. / (attack * constants::sampleRate));
	releaseGain = BhMath::exp(-1. / (release * constants::sampleRate));
}

Sample EnvelopeFollower::getPeakEnvelope(const Sample& in)
{
	return getEnvelope(getPeakSample(in));
}

Sample EnvelopeFollower::getRmsEnvelope(const Sample& in)
{
	return getEnvelope(getRmsSample(in));
}

Sample EnvelopeFollower::getCurrentEnvelope() const
{
	return envelope;
}

Sample EnvelopeFollower::getEnvelope(const Sample& envelopeSample)
{
	// TODO do this in a fancy simd way without unpacking
	alignas(16) double envelopeUnpacked[2];
	alignas(16) double envelopeSampleUnpacked[2];
	envelope.store_aligned(envelopeUnpacked);
	envelopeSample.store_aligned(envelopeSampleUnpacked);
	for (unsigned int i = 0; i < 2; i++) {
		if (envelopeUnpacked[i] < envelopeSampleUnpacked[i]) {
			envelopeUnpacked[i] = envelopeSampleUnpacked[i] + attackGain * (envelopeUnpacked[i] - envelopeSampleUnpacked[i]);
		}
		else {
			envelopeUnpacked[i] = envelopeSampleUnpacked[i] + releaseGain * (envelopeUnpacked[i] - envelopeSampleUnpacked[i]);
		}
	}
	envelope.load_aligned(envelopeUnpacked);
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