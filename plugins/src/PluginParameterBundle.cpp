#include "PluginParameterBundle.h"

#include "ParameterBundle.h"

PluginParameterBundle::PluginParameterBundle(ParameterBundle* params)
	: pluginParameters(std::vector<std::atomic<float>>(params->getNumberOfParameters()))
	, parameterConversions(params)
{
	for (unsigned int i = 0u; i < pluginParameters.size(); i++)
	{
		pluginParameters[i] = params->getParameter(i)->getDefaultValueNormalized();
	}
};

void PluginParameterBundle::setPluginParameter(unsigned int const& in, float const& unnormalizedValue)
{
	if (in >= pluginParameters.size()) return;
	pluginParameters[in] = unnormalizedValue;
}

void PluginParameterBundle::updateParameters()
{
	for (unsigned int i = 0; i < pluginParameters.size(); i++)
	{
		(parameterConversions)->getParameter(i)->setTargetValueNormalized(pluginParameters[i]);
	}
};

std::string PluginParameterBundle::getPluginParameterName(unsigned int const& in)
{
	if (in > pluginParameters.size()) return "error argument too big";
	else return (parameterConversions)->getParameter(in)->getName();
};

float PluginParameterBundle::getParameterNormalized(unsigned int const& index)
{
	return this->pluginParameters[index];
}

float PluginParameterBundle::getParameterUnnormalized(unsigned int const& index)
{
	return this->parameterConversions->getParameter(index)->fromNormalized(this->pluginParameters[index]);
}

char* PluginParameterBundle::getParameterUnit(unsigned int const& index)
{
	std::string unit = (parameterConversions)->getParameter(index)->getUnit();
	char* willBeReturned = new char[8u];
	strncpy(willBeReturned, unit.c_str(), 8);
	return willBeReturned;
}

char* PluginParameterBundle::getParameterNameCstr(unsigned int const& index)
{
	std::string unit = (parameterConversions)->getParameter(index)->getName(8);
	char* willBeReturned = new char[8u];
	strncpy(willBeReturned, unit.c_str(), 8);
	return willBeReturned;
}