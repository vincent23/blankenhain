#include "BhMath.h"

namespace BhMath {
	float log2(float x)
	{
		__asm
		{
			fld1
			fld dword ptr[x]
			fyl2x
			fstp dword ptr[x]
		}
		return x;
	}

	float log10(float x)
	{
		__asm
		{
			fld1
			fld dword ptr[x]
			fyl2x
			fldl2t
			fdiv
			fstp dword ptr[x]
		}
		return x;
	}

	float exp2(float x)
	{
		__asm
		{
			fld dword ptr[x]
			fld1
			fld	st(1)
			fprem
			f2xm1
			faddp st(1), st(0)
			fscale
			fstp st(1)
			fstp dword ptr[x]
		}
		return x;
	}

	float exp(float x)
	{
		__asm
		{
			fld dword ptr[x]
			fldl2e
			fmulp st(1), st
			fld1
			fld st(1)
			fprem
			f2xm1
			faddp st(1), st
			fscale
			fstp st(1)
			fstp dword ptr[x]
		}
		return x;
	}

	float fmod(float x, float y)
	{
		float result = 0.f;
		__asm
		{
			fld dword ptr[y]
			fld dword ptr[x]
			fprem
			fstp st(1)
			fstp dword ptr[result]
		}
		return result;
	}

	float pow(float x, float y)
	{
		float result = 0.f;
		__asm
		{
			fld dword ptr[y]
			fld dword ptr[x]
			fyl2x
			fld1
			fld	st(1)
			fprem
			f2xm1
			faddp st(1), st(0)
			fscale
			fstp st(1)
			fstp dword ptr[result]
		}
		return result;
	}

	float sqrt(float x)
	{
		__asm
		{
			fld dword ptr[x]
			fsqrt
			fstp dword ptr[x]
		}
		return x;
	}

	float tan(float x)
	{
		__asm
		{
			fld	dword ptr[x]
			fptan
			fstp st(0)
			fstp dword ptr[x]
		}
		return x;
	}

	float sin(float x)
	{
		__asm
		{
			fld dword ptr[x]
			fsin
			fstp dword ptr[x]
		}
		return x;
	}

	float abs(float x)
	{
		__asm
		{
			fld x
			fabs
			fstp x
		}
		return x;
	}
}