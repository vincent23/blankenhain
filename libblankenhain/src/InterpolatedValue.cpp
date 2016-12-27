#include "InterpolatedValue.h"

template InterpolatedValue<float>;

template <typename T>
InterpolatedValue<T>::InterpolatedValue(T from, T to, unsigned int steps)
	: current(from), targetNumSteps(steps)
	, step((to - from) / steps)
{ }

template <typename T>
InterpolatedValue<T>::InterpolatedValue(T singularValue)
	: current(singularValue), targetNumSteps(0)
	, step(0)
{ }

template <typename T>
T InterpolatedValue<T>::get(unsigned int offset)
{
	// Clamp interpolation if we reached targetValue
	if (offset == targetNumSteps)
	{
		current = current + offset * step;
		step = T(0.f);
	}
	return current + offset * step;
}

/// Slightly less efficient but const version
template <typename T>
T InterpolatedValue<T>::get(unsigned int offset) const
{
	// Clamp interpolation if we reached targetValue
	if (offset >= targetNumSteps)
	{
		return current;
	}
	return current + offset * step;
}

template <typename T>
T InterpolatedValue<T>::next(unsigned int steps)
{
	current += step * steps;
	return current;
}