#include "InterpolatedValue.h"

InterpolatedValue::InterpolatedValue(float from, float to, unsigned int steps)
	: current(from)
	, step((to - from) / steps)
{ }

float InterpolatedValue::get(unsigned int offset) const
{
	return current + offset * step;
}

void InterpolatedValue::next(unsigned int steps)
{
	current += step * steps;
}