#pragma once

namespace BhMath {
	inline float log2(float x)
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

	inline float log10(float x)
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

	inline float exp2(float x)
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

	inline float exp(float x)
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

	inline float fmod(float x, float y)
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

	inline float abs(float x)
	{
		__asm
		{
			fld x
			fabs
			fstp x
		}
		return x;
	}

	// for convenience, we implement pow as |x|^y
	//
	// This code raises FPU exception "precision" @TODO
	inline float pow(float x, float y)
	{
		x = abs(x) + static_cast<float>(1e-8);
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

	inline float sqrt(float x)
	{
		__asm
		{
			fld dword ptr[x]
			fsqrt
			fstp dword ptr[x]
		}
		return x;
	}

	inline float tan(float x)
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

	inline float sin(float x)
	{
		__asm
		{
			fld dword ptr[x]
			fsin
			fstp dword ptr[x]
		}
		return x;
	}
}