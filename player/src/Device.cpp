#include "Device.h"

#include "Constants.h"

Device::Device()
	: outputBuffer(new Sample[constants::parameterInterpolationLength])
{}

Device::~Device()
{
	if (outputBuffer != nullptr)
	{
		delete[] outputBuffer;
	}
}