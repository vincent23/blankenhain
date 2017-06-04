#include "FpuState.h"

#include <emmintrin.h>

// Function to create denormal and see if exception is raised
void make_denormal()
{
	__m128   denormal;
	int      den_vec[4] = { 1,1,1,1 };
	memcpy(&denormal, den_vec, sizeof(int) * 4);
	denormal = _mm_add_ps(denormal, denormal);
}



FpuState::FpuState() {
	// Save previous controlWord internally
#ifdef _LIBBLANKENHAIN_ENABLE_FPU_CONTROL_WORD_CHECK
	previousControlRegisterSSE = _mm_getcsr();
	previousControlRegisterFPU = 0u;
	unsigned short tempStorage = 0u;
	__asm fstcw tempStorage;
	previousControlRegisterFPU = tempStorage;
#endif
	unsigned short fpucontrolword = previousControlRegisterFPU;
	//fpucontrolword |= 0 << 7
	//fpucontrolword |= 0 << 0
	//fpucontrolword |= 1 << 1
	//fpucontrolword |= 0 << 2
	//fpucontrolword |= 0 << 3
	//fpucontrolword |= 1 << 4
	//fpucontrolword |= 1 << 5
	//fpucontrolword |= 0 << 8
	//fpucontrolword |= 0 << 9
	//fpucontrolword |= 1 << 10
	//fpucontrolword |= 1 << 11

	fpucontrolword ^= (-0 ^ fpucontrolword) & (1 << 7); // IEM
	fpucontrolword ^= (-0 ^ fpucontrolword) & (1 << 0); // IM
	fpucontrolword ^= (-1 ^ fpucontrolword) & (1 << 1); // DM
	fpucontrolword ^= (-0 ^ fpucontrolword) & (1 << 2); // ZM
	fpucontrolword ^= (-0 ^ fpucontrolword) & (1 << 3); // OF
	fpucontrolword ^= (-1 ^ fpucontrolword) & (1 << 4); // UF
	fpucontrolword ^= (-1 ^ fpucontrolword) & (1 << 5); // PF
	fpucontrolword ^= (-0 ^ fpucontrolword) & (1 << 8); // PC
	fpucontrolword ^= (-0 ^ fpucontrolword) & (1 << 9); // PC
	fpucontrolword ^= (-1 ^ fpucontrolword) & (1 << 10); // RC
	fpucontrolword ^= (-1 ^ fpucontrolword) & (1 << 11); // RC
	// Set desired control word
	__asm fldcw fpucontrolword;
#ifdef _LIBBLANKENHAIN_ENABLE_FPU_CONTROL_WORD_CHECK
	desiredControlRegisterFPU = fpucontrolword;
#endif
	// Clear exceptions
	__asm fclex;


	// SSE Stuff
	// https://software.intel.com/en-us/articles/x87-and-sse-floating-point-assists-in-ia-32-flush-to-zero-ftz-and-denormals-are-zero-daz
	unsigned long storeMXCSR;
	__asm STMXCSR storeMXCSR;
	storeMXCSR |= 1 << 6; // DAZ Mode 
	storeMXCSR |= 1 << 8; // set Denormal mask
	storeMXCSR |= 1 << 15; // FTZ mode
	storeMXCSR |= 1 << 11; // set Underflow mask
	__asm LDMXCSR storeMXCSR;
#ifdef _LIBBLANKENHAIN_ENABLE_FPU_CONTROL_WORD_CHECK
	desiredControlRegisterSSE = storeMXCSR;
#endif
	// Test!
	//make_denormal();
}

FpuState::~FpuState() {
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	unsigned short statusWord;
	__asm fstsw statusWord;
	unsigned short controlWord;
	__asm fstcw controlWord;
	unsigned int controlWordSSE;
	controlWordSSE = _mm_getcsr();

#ifdef _LIBBLANKENHAIN_ENABLE_FPU_CONTROL_WORD_CHECK
	// Check if status word is still correct
	if (controlWord != this->desiredControlRegisterFPU)
		throw std::runtime_error("fpu control word wrong");
	if (controlWordSSE != this->desiredControlRegisterSSE)
		throw std::runtime_error("sse control word wrong");
#endif

	// Check for FPU exceptions

	if (statusWord & (1 << 6)) {
		// "stack fault"; 
		int x = 0;
		throw std::runtime_error("FPU-Exception: stack fault");
	}
	// Precission erros are fine and handled by FPU rounding mode
	//if (statusWord & (1 << 6)) {
	//	// "precision"; 
	//	int x = 0;
	//	throw std::runtime_error("FPU-Exception: precision");
	//}
	if (statusWord & (1 << 4)) {
		// "underflow"; 
		int x = 0;
		throw std::runtime_error("FPU-Exception: underflow");
	}
	if (statusWord & (1 << 3)) {
		// "overflow"; 
		int x = 0;
		throw std::runtime_error("FPU-Exception: overflow");
	}
	if (statusWord & (1 << 2)) {
		// "divide by zero"; 
		int x = 0;
		throw std::runtime_error("FPU-Exception: divide by zero");
	}
	if (statusWord & (1 << 1)) {
		// "denormal"; 
		int x = 0;
		throw std::runtime_error("FPU-Exception: denormal");
	}
	if (statusWord & (1 << 0)) {
		// "invalid operation"; 
		int x = 0;
		throw std::runtime_error("FPU-Exception: invalid operation");
	}

	// @TODO check for sse exception flags

	// Clear exceptions
	__asm fclex;

#endif
}