#pragma once

#include "Device.h"

class CombinedDevice : public Device
{
public:
	CombinedDevice(unsigned int numberOfDevices, Device** devices);

protected:
	unsigned int numberOfDevices;
	Device** devices;
};