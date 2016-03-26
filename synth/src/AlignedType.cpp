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

void* AlignedType::operator new (unsigned int size)
{
	//Handle size == 0, required by cpp reference
	if (size == 0) { size = 1; }
	void *result;
	#ifdef _MSC_VER 
			//Hier vorerst alignment = 16 HARDCODED
			//weil irgendwelche komischen fehler
			//da eh mainly SSE Alignment muss das jetzt halt erstmal so
			result = _aligned_malloc(size, 16u);
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

void* AlignedType::operator new[](unsigned int size)
{
	if (size == 0) { size = 1; }
	void *result;
	#ifdef _MSC_VER 
	//Hier vorerst alignment = 16 HARDCODED
	//weil irgendwelche komischen fehler
	//da eh mainly SSE Alignment muss das jetzt halt erstmal so
	result = _aligned_malloc(size, 16u);
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
		return;                            //null pointer
	}
	#ifdef _MSC_VER 
		_aligned_free(p);
	#endif
	return;
}

void AlignedType::operator delete[] (void *p) throw()
{
	if (p == nullptr)
	{
		return;                            //null pointer
	}
#ifdef _MSC_VER 
	_aligned_free(p);
#endif
	return;
}

unsigned int AlignedType::getAlignment(void)
{
	return alignment;
}


