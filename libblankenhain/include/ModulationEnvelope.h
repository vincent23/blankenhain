#include "InstrumentBase.h"
#include "VoiceState.h"

/**
* Creates an ADHSR Modulation curve
* is unipolar (range is from 0 to one)
*
* This can be used for modulation of parameters.
*/
class ModulationEnvelope
{
private:
	float* buffer;
	//unsigned int length;
	VoiceState dummy;
public:
	ModulationEnvelope(float attackInMs_, float releaseInMs_, float holdInMs_ = 1.f, float decayInMs_ = 1.f, float sustainInMs_ = 1.f)
		: buffer(nullptr)
	{
		attackInMs = attackInMs_;
		holdInMs = holdInMs_;
		decayInMs = decayInMs_;
		sustainInMs = sustainInMs_;
		releaseInMs = releaseInMs_;
		//length = aux::millisecToSamples(attackInMs);
		//length += aux::millisecToSamples(holdInMs);
		//length += aux::millisecToSamples(decayInMs);
		//length += aux::millisecToSamples(sustainInMs);
		//length += aux::millisecToSamples(releaseInMs);
		buffer = new float[1];

		dummy.on(0u, 69u, 100u);
	};
	~ModulationEnvelope()
	{
		if (buffer != nullptr)
		{
			delete[] buffer;
			buffer = nullptr;
		}
	}
	float get(unsigned int time)
	{
		//if (time < length)
		//{
			buffer[0] = 1.f;
			performAHDSR<float>(buffer, dummy, time, 0u, attackInMs,
				releaseInMs, holdInMs, decayInMs, sustainInMs, true);
			return buffer[0];
		//}
		//else
		//	return 0.f;
	};
	float attackInMs;
	float holdInMs;
	float decayInMs;
	float sustainInMs;
	float releaseInMs;
};
