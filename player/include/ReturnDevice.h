#include "Device.h"

class ReturnDevice : public Device
{
public:
	ReturnDevice(unsigned int returnTrackIndex);

	Sample* process(SongInfo& songInfo, const Sample* input, unsigned int globalSamplePosition) override final;

private:
	unsigned int returnTrackIndex;
};