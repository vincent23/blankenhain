#include "Sample.h"

#pragma once
#pragma warning(disable: 4324)
#include <emmintrin.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" void* __cdecl aligned_malloc(
	size_t const size,
	size_t align
);

extern "C" void __cdecl aligned_free(void* const block);

void* Sample::operator new[](unsigned int size)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (size == 0) {
		throw("Alloc of size == 0\n");
		return aligned_malloc(size, 16u);
	}
#endif
	return aligned_malloc(size, 16u);
}

void Sample::operator delete[](void *p) throw()
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (p == nullptr)
	{
		throw ("Tried to delete nullptr\n");
		return;
	}
#endif
	aligned_free(p);
}