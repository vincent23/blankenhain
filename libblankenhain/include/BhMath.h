#pragma once

#include <xmmintrin.h>

#define EXP_POLY_DEGREE 5
#define LOG_POLY_DEGREE 6

#define POLY0_single(x, c0) (c0)
#define POLY1_single(x, c0, c1) (x * POLY0_single(x, c1) + c0)
#define POLY2_single(x, c0, c1, c2) (x * POLY1_single(x, c1, c2) + c0)
#define POLY3_single(x, c0, c1, c2, c3) (x * POLY2_single(x, c1, c2, c3) + c0)
#define POLY4_single(x, c0, c1, c2, c3, c4) (x * POLY3_single(x, c1, c2, c3, c4) + c0)
#define POLY5_single(x, c0, c1, c2, c3, c4, c5) (x * POLY4_single(x, c1, c2, c3, c4, c5) + c0)

namespace BhMath {
	inline float log2(float x_f)
	{
		unsigned int exp = 0x7F800000;
		unsigned int mant = 0x007FFFFF;
		union u { float f; unsigned int i; };
		u temp;
		temp.f = x_f;
		unsigned int i = temp.i;
		auto e = static_cast<float>(static_cast<int>(((i & exp) >> 23) - 127));
		temp.f = 1.f;
		unsigned int one = temp.i;
		unsigned int foo = ((i & mant) | one);
		temp.i = foo;
		float dings = temp.f;
		auto m = dings;

		float p;

		/* Minimax polynomial fit of log2(x)/(x - 1), for x in range [1, 2[ */
#if LOG_POLY_DEGREE == 6
		p = POLY5_single(m, 3.1157899f, -3.3241990f, 2.5988452f, -1.2315303f, 3.1821337e-1f, -3.4436006e-2f);
#elif LOG_POLY_DEGREE == 5
		p = POLY4_single(m, 2.8882704548164776201f, -2.52074962577807006663f, 1.48116647521213171641f, -0.465725644288844778798f, 0.0596515482674574969533f);
#elif LOG_POLY_DEGREE == 4
		p = POLY3_single(m, 2.61761038894603480148f, -1.75647175389045657003f, 0.688243882994381274313f, -0.107254423828329604454f);
#elif LOG_POLY_DEGREE == 3
		p = POLY2_single(m, 2.28330284476918490682f, -1.04913055217340124191f, 0.204446009836232697516f);
#else
#error
#endif

		/* This effectively increases the polynomial degree by one, but ensures that log2(1) == 0*/
		p *= m - 1.f;
		return p + e;
	}

	inline float log10(float x)
	{
		float log10_2 = 0.3010299956639812f;
		return log2(x) * log10_2;
	}

	inline float exp2(float x_f)
	{
		auto x = _mm_set_ss(x_f);
		x = _mm_min_ss(x, _mm_set_ss(129.f));
		x = _mm_max_ss(x, _mm_set_ss(-126.99999f));
		_mm_store_ss(&x_f, x);

		auto ipart = static_cast<int>(x_f - 0.5f);
		auto fpart = x_f - static_cast<float>(ipart);

		union u { float f; unsigned int i; };
		u temp;
		temp.i = static_cast<unsigned int>(((ipart + 127) << 23));
		auto expipart = temp.f;

		float expfpart;

		/* minimax polynomial fit of 2**x, in range [-0.5, 0.5[ */
#if EXP_POLY_DEGREE == 5
		expfpart = POLY5_single(fpart, 9.9999994e-1f, 6.9315308e-1f, 2.4015361e-1f, 5.5826318e-2f, 8.9893397e-3f, 1.8775767e-3f);
#elif EXP_POLY_DEGREE == 4
		expfpart = POLY4_single(fpart, 1.0000026f, 6.9300383e-1f, 2.4144275e-1f, 5.2011464e-2f, 1.3534167e-2f);
#elif EXP_POLY_DEGREE == 3
		expfpart = POLY3_single(fpart, 9.9992520e-1f, 6.9583356e-1f, 2.2606716e-1f, 7.8024521e-2f);
#elif EXP_POLY_DEGREE == 2
		expfpart = POLY2_single(fpart, 1.0017247f, 6.5763628e-1f, 3.3718944e-1f);
#else
#error
#endif

		return expipart * expfpart;
	}

	inline float exp(float x)
	{
		float log2e = 1.4426950408889634f;
		return exp2(x * log2e);
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
		// TODO remove abs
		return exp2(log2(abs(x)) * y);
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