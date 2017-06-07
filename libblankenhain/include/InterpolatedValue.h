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

class InterpolatedValueBlock
{
public:
	InterpolatedValueBlock(unsigned int numberOfParameters);
	~InterpolatedValueBlock();

	/// gets current interpolated value,
	float get(unsigned int index) const;
	float get(unsigned int index, unsigned int offset) const;

	void setCurrent(unsigned int index, float value);
	void setTarget(unsigned int index, float to);
	void setSteps(unsigned int steps);

	/// Skips a number of interpolation steps
	void next(unsigned int steps = 1);

protected:
	float* current;
	float* step;
	float inverseNumberOfSteps;
	unsigned int numberOfParameters;
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	/*const*/ unsigned int targetNumSteps;
	unsigned int currentStep;
#endif
};