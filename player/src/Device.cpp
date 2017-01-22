#include "Device.h"

Device::Device(unsigned int blockSize)
	: outputBuffer(new Sample[blockSize])
{}

Device::~Device()
{
	if (outputBuffer != nullptr)
	{
		delete[] outputBuffer;
	}
}