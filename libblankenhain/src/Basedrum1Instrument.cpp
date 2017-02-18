#include "basedrum1Instrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

basedrum1Instrument::basedrum1Instrument()
	: InstrumentBase(14, 4),
	drm1(58.f, 5.f, 1.f),
	drm2(80.f, 200.f, 0.75),
	drm1_lowpassfreq(58.f),
	drm1_lowpassq(5.f),
	drm1_pulselength(1.f)
{
	ParameterBundle* params = getPointerToParameterBundle();

	params->getParameter(0) = new FloatParameter(50.f, NormalizedRange(0.001f, 1700.f, 0.3f), "attack", "ms");
	params->getParameter(1) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "hold", "ms");
	params->getParameter(2) = new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio");
	params->getParameter(3) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "decay", "ms");
	params->getParameter(4) = new FloatParameter(0.f, NormalizedRange(), "sustainBool", "bool");
	params->getParameter(5) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "sustain", "ms");
	params->getParameter(6) = new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio");
	params->getParameter(7) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "release", "ms");
	params->getParameter(8) = new FloatParameter(0.f, NormalizedRange(0.f, 1.9f), "osc", "");
	params->getParameter(9) = new FloatParameter(58.f, NormalizedRange(35.f, 5000.f, 0.35f), "1lowpass", "freq");
	params->getParameter(10) = new FloatParameter(4.f, NormalizedRange(0.1f, 5.f, 1.2f), "1q", "");
	params->getParameter(11) = new FloatParameter(1.f, NormalizedRange(0.1f, 50.f, 0.35f), "1pulse", "ms");
	params->getParameter(12) = new FloatParameter(0.75f, NormalizedRange(0.1f, 10.f, 0.3f), "2envMult", "");
	params->getParameter(13) = new FloatParameter(1.0f, NormalizedRange(0.1f, 500.f, 0.3f), "2envRelease", "ms");
}

basedrum1Instrument::~basedrum1Instrument()
{
}

void basedrum1Instrument::processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples)
{

	float attack = getInterpolatedParameter(0).get();
	float hold = getInterpolatedParameter(1).get();
	float holdLevel = getInterpolatedParameter(2).get();
	float decay = getInterpolatedParameter(3).get();
	bool sustainOn = getInterpolatedParameter(4).get() > 0.5 ? true : false;
	float sustainLevel = getInterpolatedParameter(6).get();
	float sustain = getInterpolatedParameter(5).get();
	float release = getInterpolatedParameter(7).get();
	unsigned int oscMode = static_cast<unsigned int>(getInterpolatedParameter(8).get());

	this->drm2.setFrequency(aux::noteToFrequency(static_cast<float>(voice.key)));

	if (oscMode == 0u)
	{

		if (drm1_lowpassfreq != getInterpolatedParameter(9).get())
		{
			drm1_lowpassfreq = getInterpolatedParameter(9).get();
			this->drm1.setLowPass(drm1_lowpassfreq, drm1_lowpassq);
		}
		if (drm1_lowpassq != getInterpolatedParameter(10).get())
		{
			drm1_lowpassq = getInterpolatedParameter(10).get();
			this->drm1.setLowPass(drm1_lowpassfreq, drm1_lowpassq);
		}
		if (drm1_pulselength != getInterpolatedParameter(11).get())
		{
			drm1_pulselength = getInterpolatedParameter(11).get();
			this->drm1.setPuleLength(drm1_pulselength);
		}
		if (drm1_pulselength != getInterpolatedParameter(11).get())
		{
			drm1_pulselength = getInterpolatedParameter(11).get();
			this->drm1.setPuleLength(drm1_pulselength);
		}

		this->currentGenerator = &drm1;
	}
	else
	{
		this->drm2.setEnvMult(getInterpolatedParameter(12).get());
		this->drm2.setEnvRelease(getInterpolatedParameter(13).get());

		this->currentGenerator = &drm2;

	}
	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++) {
		unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
		buffer[sampleIndex] = Sample(this->currentGenerator->getSample(deltaT));
		performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
	}
}