#pragma once

/**
 * Interpolates from a starting flaot to a target float over a given number of steps.
 * 
 * Use with T = float
 *
 * To skip interpolation steps, use next()
 * To get next interpolated value, use get()
 *
 * Stops interpolation when target value is reached.
 **/ 
template <typename T>
class InterpolatedValue {
public:
	InterpolatedValue(T from, T to, unsigned int steps);
	InterpolatedValue(T singularValue);
	InterpolatedValue() = default;

	/// gets current interpolated value,
	T get() const;
	T get(unsigned int offset) const;

	/// Skips a number of interpolation steps
	T next(unsigned int steps = 1);

private:
	T current;
	T step;
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	/*const*/ unsigned int targetNumSteps;
	unsigned int currentStep;
#endif
};