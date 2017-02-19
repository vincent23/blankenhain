#include "InterpolatedValue.h"

template InterpolatedValue<float>;

template <typename T>
InterpolatedValue<T>::InterpolatedValue(T from, T to, unsigned int steps)
	: current(from)
	, step((to - from) / steps)
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
T InterpolatedValue<T>::get() const
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
	if (currentStep + steps >= targetNumSteps)
	{
		throw "out of bounds";
	}
	currentStep += steps;
#endif
	current += step * steps;
	return current;
}