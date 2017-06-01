#include "FpuState.h"

#include <emmintrin.h>


FpuState::FpuState() {
	// clear exception flags
	previousControlRegister = _mm_getcsr() & (~0b111111);
	_mm_setcsr(
		1 << 15 | // flush to zero
		1 << 12 | // precision mask
		1 << 11 | // underflow mask
		1 << 10 | // overflow mask
		1 << 9 | // divide by zero mask
		1 << 8 | // denormal mask
		1 << 7 | // invalid operation mask
		1 << 6
	);
}

FpuState::~FpuState() {
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	unsigned int flags = _mm_getcsr() & 0b111111;
	if (flags != 0) {
		//if (flags & (1 << 5)) {
		//	// "precision";
		//	int x = 0;
		//	throw std::runtime_error("FPU-State wrong: precision");
		//}
		if (flags & (1 << 4)) {
			// "underflow";
			int x = 0;
			throw std::runtime_error("FPU-State wrong: underflow");
		}
		if (flags & (1 << 3)) {
			// "overflow";
			int x = 0;
			throw std::runtime_error("FPU-State wrong: overflow");
		}
		if (flags & (1 << 2)) {
			// "divide by zero";
			int x = 0;
			throw std::runtime_error("FPU-State wrong: divide by zero");
		}
		if (flags & (1 << 1)) {
			// "denormal";
			int x = 0;
			throw std::runtime_error("FPU-State wrong: denormal");
		}
		else {
			// "invalid operation";
			int x = 0;
			throw std::runtime_error("FPU-State wrong: invalid operation");
		}
	}
#endif
	_mm_setcsr(previousControlRegister);
#ifdef  _LIBBLANKENHAIN_ENABLE_FPU_ROUNDING_CHECK
	unsigned short bar = 0u;
	_asm FSTCW bar
	if (bar != 3711)
		throw std::runtime_error("fpu rounding flag wrong");
#endif
}