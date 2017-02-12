#include "BhMath.h"
#include "AuxFunc.h"

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

	// for convenience, we implement pow as |x|^y
	float pow(float x, float y)
	{
		x = abs(x);
		if (x < 1e-8) {
			return x;
		}
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

	//via http://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
	double fastPow(double a, double b) {
		union {
			double d;
			int x[2];
		} u = { a };
		u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
		u.x[0] = 0;
		return u.d;
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