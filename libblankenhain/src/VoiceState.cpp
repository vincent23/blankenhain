#include "VoiceState.h"

void VoiceState::on(unsigned int time, unsigned int key_, unsigned int velocity_)
{
	isOn = true;
	isSilent = false;
	key = key_;
	velocity = velocity_;
	onTime = time;
}

void VoiceState::off(unsigned int time)
{
	isOn = false;
	offTime = time;
}

unsigned int VoiceState::time() const
{
	return isOn ? onTime : offTime;
}