#pragma once

class AlignedType
{
	protected:
		const unsigned int alignment = 16;
	public:
		AlignedType() {};
		void *operator new (unsigned int size);
		void *operator new[](unsigned int size);
		void operator delete (void *p) throw();
		void operator delete[](void *p);
		unsigned int getAlignment();
};
