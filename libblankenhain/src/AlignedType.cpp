#include <malloc.h>
#include "AlignedType.h"

//Notes for future maintenance and dev:
//This would be a smart approach:

/*If _STDC_VERSION__ >= 201112L use aligned_alloc.
If _POSIX_VERSION >= 200112L use posix_memalign.
If _MSC_VER is defined, use the Windows stuff.
...
If all else fails, just use malloc / free and disable SSE / AVX code.*/
//return _mm_malloc(size, 16);

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" void* __cdecl aligned_malloc(
	size_t const size,
	size_t align
)
{
	const size_t offset = 0;
	uintptr_t ptr, retptr, gap;
	size_t nonuser_size, block_size;

	align = (align > sizeof(void*) ? align : sizeof(void*)) - 1;

	/* gap = number of bytes needed to round up offset to align with PTR_SZ*/
	gap = (0 - offset)&(sizeof(void*) - 1);

	nonuser_size = sizeof(void*) + gap + align;
	block_size = nonuser_size + size;

		if ((ptr = (uintptr_t)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, block_size)) == (uintptr_t)nullptr)
			return nullptr;

	retptr = ((ptr + nonuser_size + offset)&~align) - offset;
	((uintptr_t *)(retptr - gap))[-1] = ptr;

	return (void *)retptr;
}

extern "C" void __cdecl aligned_free(void* const block)
{
	uintptr_t ptr;

	if (block == nullptr)
		return;

	ptr = (uintptr_t)block;

	/* ptr points to the pointer to starting of the memory block */
	ptr = (ptr & ~(sizeof(void*) - 1)) - sizeof(void*);

	/* ptr is the pointer to the start of memory block*/
	ptr = *((uintptr_t*)ptr);
	if (ptr) HeapFree(GetProcessHeap(), 0, (void*) ptr);
}

#pragma warning (push)
#pragma warning (disable : 4702)
void* AlignedType::operator new (unsigned int size)
{
	//Handle size == 0, required by cpp reference
	if (size == 0) { size = 1; }
	void *result;
#ifdef _MSC_VER
	//Hier vorerst alignment = 16 HARDCODED
	//weil irgendwelche komischen fehler
	//da eh mainly SSE Alignment muss das jetzt halt erstmal so
	result = aligned_malloc(size, 16u);
#else
	if (posix_memalign(&result, 16u, size))
	{
		result = 0;
	}
#endif
	return result;
	//Use std::new in case something goes wrong
	return ::operator new(size);
}
#pragma warning (pop)

void* AlignedType::operator new[](unsigned int size)
{
	if (size == 0) { size = 1; }
	void *result;
#ifdef _MSC_VER
	//Hier vorerst alignment = 16 HARDCODED
	//weil irgendwelche komischen fehler
	//da eh mainly SSE Alignment muss das jetzt halt erstmal so
	result = aligned_malloc(size, 16u);
#else
	if (posix_memalign(&result, 16u, size))
	{
		result = 0;
	}
#endif
	return result;
	//Use std::new in case something goes wrong
	//return ::operator new(size);
}

void AlignedType::operator delete (void *p) throw()
{
	if (p == nullptr)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		throw ("Tried to delete nullptr\n");
#endif
		return;
	}

#ifdef _MSC_VER
	aligned_free(p);
#endif
	return;
}

void AlignedType::operator delete[](void *p) throw()
{

	if (p == nullptr)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		throw ("Tried to delete nullptr\n");
#endif
		return;
	}

#ifdef _MSC_VER
	aligned_free(p);
#endif
	return;
}

unsigned int AlignedType::getAlignment(void) const
{
	//return alignment;
	return 16;
}


