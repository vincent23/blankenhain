#ifndef ALIGNEDTYPE_H_INCLUDED
#define ALIGNEDTYPE_H_INCLUDED
class AlignedType
{
	private:
		const unsigned int alignment = 16;
	public:
		AlignedType() {};
		void *operator new (unsigned int size);
		void *operator new[](unsigned int size);
		void operator delete (void *p) throw();
		void operator delete[](void *p);
		unsigned int getAlignment();
};
#endif