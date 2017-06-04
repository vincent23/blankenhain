#pragma once
#include "Options.h"


/**
* RAII class for setting the right FPU flags (flush to zero,
* denormals are zero, no exceptions).
* Just create an object of this class in the scope where you do
* the calculations.
*
* More information of FPU via this very nice website
* http://www.website.masmforum.com/tutorials/fptute/fpuchap1.htm
* http://www.website.masmforum.com/tutorials/fptute/fpuchap2.htm
* http://www.website.masmforum.com/tutorials/fptute/fpuchap3.htm
*/
class FpuState final {
public:
	FpuState();
	~FpuState();

private:
#ifdef _LIBBLANKENHAIN_ENABLE_FPU_CONTROL_WORD_CHECK
	unsigned int previousControlRegisterSSE;
	unsigned short previousControlRegisterFPU;
	unsigned short desiredControlRegisterFPU = 0;
	unsigned int desiredControlRegisterSSE = 0;
#endif
	
};
