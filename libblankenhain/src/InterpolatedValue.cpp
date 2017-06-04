#include "InterpolatedValue.h"

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
