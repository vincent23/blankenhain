#include "CombinedDevice.h"

CombinedDevice::CombinedDevice(unsigned int blockSize, unsigned int numberOfDevices_, Device* devices_)
	: Device(blockSize)
	, numberOfDevices(numberOfDevices_)
	, devices(devices_)
{}