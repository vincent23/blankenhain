#include "InterpolatedValue.h"

#include <xmmintrin.h>
#include <emmintrin.h>

template InterpolatedValue<float>;
template BoundrySafeInterpolatedValue<float>;

template <typename T>
InterpolatedValue<T>::InterpolatedValue(T from, T to, unsigned int steps)
	: current(from)
	, step((to - from) / static_cast<T>(steps))
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	, targetNumSteps(steps)
	, currentStep(0u)
#endif
{ }

template <typename T>
InterpolatedValue<T>::InterpolatedValue(T singularValue)
	: current(singularValue)
	, step(0)
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	, targetNumSteps(0)
	, currentStep(0u)
#endif
{ }

template <typename T>
T InterpolatedValue<T>::get(void) const
{
	return current;
}

template <typename T>
T InterpolatedValue<T>::get(unsigned int offset) const
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (currentStep == targetNumSteps) {
		throw "out of bounds";
	}
#endif
	return current + offset * step;
}

template <typename T>
T InterpolatedValue<T>::next(unsigned int steps)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (currentStep + steps > targetNumSteps
		&& step != 0.f)
	{
		throw "out of bounds";
	}
	currentStep += steps;
#endif
	current += step * steps;
	return current;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////												//////////
//////////////		BOUNDRY CHECKED INTERPOLATED VALUE		//////////
//////////////												//////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


template <typename T>
BoundrySafeInterpolatedValue<T>::BoundrySafeInterpolatedValue(T from, T to, unsigned int steps)
	: InterpolatedValue(from, to, steps)
	, targetNumSteps(steps)
	, currentStep(0u)
{ }

template <typename T>
BoundrySafeInterpolatedValue<T>::BoundrySafeInterpolatedValue(T singularValue)
	: InterpolatedValue(singularValue)
	, targetNumSteps(0u)
	, currentStep(0u)
{ }

template <typename T>
T BoundrySafeInterpolatedValue<T>::get(unsigned int offset) const
{
	if (currentStep + offset >= targetNumSteps)
	{
		return current;
	}
	else
	{
		return current + offset * step;
	}

}

template <typename T>
T BoundrySafeInterpolatedValue<T>::next(unsigned int steps)
{
	// Check if we bound overshoot our boundries with this next
	if (currentStep + steps >= targetNumSteps)
	{
		// if we do, check if we ONLY overshoot since this step (and therefore
		// we need to still ramp up current to its target value)
		if (currentStep < targetNumSteps)
		{
			current += step * (targetNumSteps - currentStep);
			currentStep = targetNumSteps;
		}
		return current;
	}
	else
	{
		// we dont overshoot, so we ramp normal
		currentStep += steps;
		current += step * steps;
		return current;
	}
}

extern "C" void* __cdecl aligned_malloc(
	size_t const size,
	size_t align
);

extern "C" void __cdecl aligned_free(void* const block);

InterpolatedValueBlock::InterpolatedValueBlock(unsigned int numberOfParameters_)
	: current(nullptr)
	, step(nullptr)
	, numberOfParameters((numberOfParameters_ + 3) & ~3) // round up to multiple of 4 (for SSE)
{
	if (numberOfParameters != 0) {
		current = static_cast<float*>(aligned_malloc(numberOfParameters * sizeof(float), 16u));
		step = static_cast<float*>(aligned_malloc(numberOfParameters * sizeof(float), 16u));
	}
}

InterpolatedValueBlock::~InterpolatedValueBlock()
{
	if (numberOfParameters != 0) {
		aligned_free(current);
		aligned_free(step);
	}
}

float InterpolatedValueBlock::get(unsigned int index) const
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (index >= numberOfParameters) {
		throw "index out of bounds";
	}
#endif
	return current[index];
}

float InterpolatedValueBlock::get(unsigned int index, unsigned int offset) const
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (currentStep + offset >= targetNumSteps) {
		throw "offset out of bounds";
	}
	if (index >= numberOfParameters) {
		throw "index out of bounds";
	}
#endif
	return current[index] + offset * step[index];
}

void InterpolatedValueBlock::setCurrent(unsigned int index, float value)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (index >= numberOfParameters) {
		throw "index out of bounds";
	}
#endif
	current[index] = value;
	step[index] = 0;
}

void InterpolatedValueBlock::setTarget(unsigned int index, float to)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (index >= numberOfParameters) {
		throw "index out of bounds";
	}
#endif
	step[index] = (to - current[index]) * inverseNumberOfSteps;
}

void InterpolatedValueBlock::setSteps(unsigned int steps)
{
	__m128 s = _mm_set_ss(static_cast<float>(steps));
	__m128 rcp = _mm_rcp_ss(s);
	_mm_store_ss(&inverseNumberOfSteps, rcp);
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	currentStep = 0;
	targetNumSteps = steps;
#endif
}

void InterpolatedValueBlock::next(unsigned int steps)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (currentStep + steps > targetNumSteps
		&& step != 0.f)
	{
		throw "steps out of bounds";
	}
	currentStep += steps;
#endif
	__m128i stepsPI = _mm_set1_epi32(steps);
	__m128 stepsPS = _mm_cvtepi32_ps(stepsPI);
	for (unsigned int i = 0; i < numberOfParameters; i += 4) {
		__m128 stepPS = _mm_load_ps(step + i);
		__m128 currentPS = _mm_load_ps(current + i);
		__m128 newPS = _mm_add_ps(currentPS, _mm_mul_ps(stepPS, stepsPS));
		_mm_store_ps(current + i, newPS);
	}
}