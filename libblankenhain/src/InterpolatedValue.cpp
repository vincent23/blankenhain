#include "InterpolatedValue.h"

template InterpolatedValue<float>;

template <typename T>
InterpolatedValue<T>::InterpolatedValue(T from, T to, unsigned int steps)
	: current(from), targetNumSteps(steps)
	, step((to - from) / steps), currentStep(0u)
{ }

template <typename T>
InterpolatedValue<T>::InterpolatedValue(T singularValue)
	: current(singularValue), targetNumSteps(0)
	, step(0), currentStep(0u)
{ }

template <typename T>
T InterpolatedValue<T>::get(unsigned int offset) const
{
	if (currentStep == targetNumSteps)
		return current;
	else
	{
		return current + offset * step;
	}
}

template <typename T>
T InterpolatedValue<T>::next(unsigned int steps)
{
	if (currentStep >= targetNumSteps)
	{
		return current;
	}
	else
	{
		currentStep += steps;
	}
	current += step * steps;
	return current;
}