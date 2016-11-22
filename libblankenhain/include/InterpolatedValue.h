#pragma once

class InterpolatedValue {
public:
	InterpolatedValue() = default;
	InterpolatedValue(float from, float to, unsigned int steps);

	float get(unsigned int offset = 0) const;
	void next(unsigned int steps = 1);

private:
	float current;
	float step;
};