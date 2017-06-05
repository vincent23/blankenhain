#pragma once
#include "Options.h"

/**
 * Interpolates from a starting flaot to a target float over a given number of steps.
 * 
 * Use with T = float
 *
 * To perform interpolation steps, use next()
 * To merely "get" the next interpolated value ("take a peek"), without actually ramping up, use get()
 *
 * InterpolatedValue is lean and mean and will not check boundries.
 * You may well crash the blankenhain by calling next() past the target value.
 * 
 * If you need a boundry safe version, where calling next() past the target interpolation value
 * will do nothing (no overshooting the targetvalue), use BoundrySafeInterpolatedValue.
 **/ 

template <typename T>
class InterpolatedValue 
{
public:
	InterpolatedValue(T from, T to, unsigned int steps);
	InterpolatedValue(T singularValue);
	InterpolatedValue() = default;

	/// gets current interpolated value,
	T get() const;
	virtual T get(unsigned int offset) const;

	/// Skips a number of interpolation steps
	virtual T next(unsigned int steps = 1);

protected:
	T current;
	T step;
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	/*const*/ unsigned int targetNumSteps;
	unsigned int currentStep;
#endif
};

template <typename T>
class BoundrySafeInterpolatedValue : public InterpolatedValue<T>
{
public:
	BoundrySafeInterpolatedValue(T from, T to, unsigned int steps);
	BoundrySafeInterpolatedValue(T singularValue);
	BoundrySafeInterpolatedValue() = default;

	T get(unsigned int offset = 0u) const override;
	T next(unsigned int steps = 1u) override;
protected:
	unsigned int targetNumSteps;
	unsigned int currentStep;
};
