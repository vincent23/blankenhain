#pragma once

#include <string>
#include <vector>
#include <atomic>

class ParameterBundle;

class PluginParameterBundle
{
public:
	PluginParameterBundle(ParameterBundle* params);

	void setPluginParameter(unsigned int const& in, float const& unnormalizedValue);
	void updateParameters();
	std::string getParameterName(unsigned int const& in);
	float getParameterNormalized(unsigned int const& index);
	float getParameterUnnormalized(unsigned int const& index);
	std::string getParameterUnit(unsigned int const& index);

private:
	std::vector<std::atomic<float>> pluginParameters;
	ParameterBundle* parameterConversions;
};