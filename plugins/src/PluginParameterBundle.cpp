#include "PluginParameterBundle.h"

#include "ParameterBundle.h"

PluginParameterBundle::PluginParameterBundle(ParameterBundle* params)
	: currentParametersFromHost(std::vector<std::atomic<float>>(params->getNumberOfParameters()))
	, parameterBundle(params)
{
	for (unsigned int i = 0u; i < currentParametersFromHost.size(); i++)
	{
		currentParametersFromHost[i] = params->getParameter(i)->getDefaultValueNormalized();
	}
};

void PluginParameterBundle::setPluginParameter(unsigned int const& in, float const& normalizedValue)
{
	if (in >= currentParametersFromHost.size()) return;
	currentParametersFromHost[in] = normalizedValue;
}

void PluginParameterBundle::updateParameters()
{
	for (unsigned int i = 0; i < currentParametersFromHost.size(); i++)
	{
		(parameterBundle)->getParameter(i)->setTargetValueNormalized(currentParametersFromHost[i]);
	}
};

unsigned int PluginParameterBundle::getNumberOfParameters() const
{
	return this->currentParametersFromHost.size();
}

std::string PluginParameterBundle::getParameterName(unsigned int const& in) const
{
	if (in > currentParametersFromHost.size()) return "error argument too big";
	else return (parameterBundle)->getParameter(in)->getName();
};

float PluginParameterBundle::getParameterNormalized(unsigned int const& index) const
{
	return this->currentParametersFromHost[index];
}

float PluginParameterBundle::getParameterUnnormalized(unsigned int const& index) const
{
	return this->parameterBundle->getParameter(index)->fromNormalized(this->currentParametersFromHost[index]);
}

std::string PluginParameterBundle::getParameterUnit(unsigned int const& index) const
{
	return parameterBundle->getParameter(index)->getUnit();
}

const ParameterBundle& PluginParameterBundle::getParameterBundle() const
{
	return *parameterBundle;
}
