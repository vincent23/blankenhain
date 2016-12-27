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

	/// gets next interpolated value
	T get(unsigned int offset = 0);

	/// gets next interpolated value,
	/// less efficient but const
	T get(unsigned int offset = 0) const;

	/// Skips a number of interpolation steps
	T next(unsigned int steps = 1);

private:
	T current;
	T step;
	unsigned int targetNumSteps;
};