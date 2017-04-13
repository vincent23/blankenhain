#include "Device.h"

class ParameterTrack;

class SendDevice : public Device
{
public:
	SendDevice(ParameterTrack* parameterValues);

	Sample* process(SongInfo& songInfo, const Sample* input, unsigned int globalSamplePosition) override final;

private:
	float sendToDb(float send) const;

	ParameterTrack* parameterValues;
};