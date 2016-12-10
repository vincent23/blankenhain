#pragma once

/*
 * Interface class to yield aligned data on the heap
 *
 * If a class is derived from AlignedType, even data
 * dynamically allocated will be aligned to 16-bit marks.
 *
 * @todo Enhance AlignedType to work with arbitrary alignments
 *
 * This is mainly used by structures working on or handling SSE
 * data, such as the "Sample" object.
 */
class AlignedType
{
protected:
public:
	AlignedType() {};
	void *operator new (unsigned int size);
	void *operator new[](unsigned int size);
	void operator delete (void *p) throw();
	void operator delete[](void *p);
	unsigned int getAlignment();
};
