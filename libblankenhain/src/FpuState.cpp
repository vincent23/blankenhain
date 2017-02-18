#include "FpuState.h"

#include <emmintrin.h>

FpuState::FpuState() {
	previousControlRegister = _mm_getcsr();

	// bits: 15 = flush to zero | 6 = denormals are zero
	// bitwise-OR with exception masks 12:7 (exception flags 5:0)
	// rounding 14:13, 00 = nearest, 01 = neg, 10 = pos, 11 = to zero
	const unsigned int mode = 0; // nearest
	_mm_setcsr(0x8040 | 0x1f80 | (mode << 13));
}

FpuState::~FpuState() {
	// clear exception flags before restoring
	_mm_setcsr(previousControlRegister & (~0x3f));
}