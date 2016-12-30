#pragma once

class VoiceState
{
public:
	void on(unsigned int time, unsigned int key, unsigned int velocity);
	void off(unsigned int time);
	unsigned int time() const;

	bool isOn = false;
	bool isSilent = true;
	unsigned int key;
	unsigned int velocity;
	unsigned int onTime;
	unsigned int offTime;
};