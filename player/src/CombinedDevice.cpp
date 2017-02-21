#include "CombinedDevice.h"

CombinedDevice::CombinedDevice(unsigned int numberOfDevices_, Device** devices_)
	: numberOfDevices(numberOfDevices_)
	, devices(devices_)
{}