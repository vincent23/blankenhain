#include "FpuState.h"

#include <emmintrin.h>

#ifndef _LIBBLANKENHAIN_ENABLE_FPU_ROUNDING_CHECK
//#define  _LIBBLANKENHAIN_ENABLE_FPU_ROUNDING_CHECK
#endif

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
		if (flags & (1 << 5)) {
			// "precision";
			int x = 0;
		}
		else if (flags & (1 << 4)) {
			// "underflow";
			int x = 0;
		}
		else if (flags & (1 << 3)) {
			// "overflow";
			int x = 0;
		}
		else if (flags & (1 << 2)) {
			// "divide by zero";
			int x = 0;
		}
		else if (flags & (1 << 1)) {
			// "denormal";
			int x = 0;
		}
		else {
			// "invalid operation";
			int x = 0;
		}
	}
#endif
	_mm_setcsr(previousControlRegister);
#ifdef  _LIBBLANKENHAIN_ENABLE_FPU_ROUNDING_CHECK
	unsigned short bar = 0u;
	_asm FSTCW bar
	if (bar != 3711)
		throw "fpu rounding flag wrong";
#endif
}