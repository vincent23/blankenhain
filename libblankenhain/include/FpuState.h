#pragma once
#include "Options.h"


/**
* RAII class for setting the right FPU flags (flush to zero,
* denormals are zero, no exceptions).
* Just create an object of this class in the scope where you do
* the calculations.
*/
class FpuState final {
public:
	FpuState();
	~FpuState();

private:
	unsigned int previousControlRegister;
};