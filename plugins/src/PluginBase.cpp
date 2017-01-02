#include "PluginBase.h"
#include "EffectBase.h"
#include "PluginParameterBundle.h"
#include "Sample.h"
#include "Constants.h"
#include "ImguiEffectEditor.h"

#include <string>
#include <algorithm>

PluginBase::PluginBase(audioMasterCallback const& audioMaster, EffectBase* effect_, bool producesTailOutput)
	: AudioEffectX(audioMaster, 1, effect_->getNumberOfParameters())
	, speakerArr(nullptr)
	, effect(effect_)
	, pluginParameters(new PluginParameterBundle(effect_->getPointerToParameterBundle()))
	, processBuffer(new Sample[constants::blockSize])
{
	canDoubleReplacing(false);
	setNumInputs(2);
	setNumOutputs(2);
	allocateArrangement(&speakerArr, 2);
	speakerArr->type = kSpeakerArrStereo;
	speakerArr->numChannels = 2;
	canProcessReplacing();
	noTail(!producesTailOutput);
	isSynth(false);

	//for now
	programsAreChunks(false);

	speakerArr = nullptr;

	//	virtual void setInitialDelay (VstInt32 delay);		///< Use to report the plug-in's latency (Group Delay)

};

PluginBase::~PluginBase()
{
	deallocateArrangement(&speakerArr);
	if (effect != nullptr) delete effect;
	effect = nullptr;
	if (pluginParameters != nullptr) delete pluginParameters;
	pluginParameters = nullptr;
}



bool PluginBase::canParameterBeAutomated(VstInt32 index)
{
	return true;
}

bool PluginBase::getParameterProperties(VstInt32 index, VstParameterProperties* p)
{
	if (index < this->getParameters().getNumberOfParameters())
	{
		VstParameterProperties& prop = *p;
		prop.flags = kVstParameterSupportsDisplayIndex + kVstParameterSupportsDisplayCategory;

		if (this->getParameters().getParameterBundle().getParameter(index)->isBoolean())
			prop.flags += kVstParameterIsSwitch;

		prop.displayIndex = index;
		prop.category = index;

		std::string text = pluginParameters->getParameterName(index);
		vst_strncpy(prop.categoryLabel, text.c_str(), std::min(text.length(), size_t(kVstMaxCategLabelLen)));
		vst_strncpy(prop.label, text.c_str(), std::min(text.length(), size_t(kVstMaxLabelLen)));
		vst_strncpy(prop.shortLabel, text.c_str(), std::min(text.length(), size_t(kVstMaxShortLabelLen)));
		return true;
	}
	return false;
} ///< Return parameter properties

bool PluginBase::string2parameter(VstInt32 index, char* text)
{
	//Convert a string representation to a parameter value.
	//
	//  Especially useful for plug - ins without user interface.The application can then implement a text edit field for the user to set a parameter by entering text.
	//
	//  Parameters :
	//  index 	Index of the parameter
	//  text 	A textual description of the parameter's value. A NULL pointer is used to check the capability (return true).
	//  Returns :
	//  true on success
	//  Note :
	//Implies setParameter().text == 0 is to be expected to check the capability(returns true)

	if (index < this->getParameters().getNumberOfParameters())
	{
		float floatValueUnnormalized = atof(text);
		if (pluginParameters->getParameterBundle().getParameter(index)->isInRange(floatValueUnnormalized))
		{
			float floatValueNormalized = pluginParameters->getParameterBundle().getParameter(index)->toNormalized(floatValueUnnormalized);
			this->pluginParameters->setPluginParameter(static_cast<unsigned int>(index), floatValueNormalized);

			return true;
		}
	}
	return false;
}

bool PluginBase::setSpeakerArrangement(VstSpeakerArrangement* pluginInput, VstSpeakerArrangement* pluginOutput)
{
	if (pluginInput != nullptr
		&& pluginOutput != nullptr)
	{
		if (
			pluginInput->type == kSpeakerArrStereo &&
			pluginOutput->type == kSpeakerArrStereo)
			return true;
		else return false;
	}
	else return false;

} ///< Set the plug-in's speaker arrangements

float PluginBase::getSampleRate()
{
	return constants::sampleRate;
}

bool PluginBase::getSpeakerArrangement(VstSpeakerArrangement** pluginInput, VstSpeakerArrangement** pluginOutput)
{
	pluginInput = &speakerArr;
	pluginOutput = &speakerArr;
	return true;
} ///< Return the plug-in's speaker arrangements

void PluginBase::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames_)
{
	if (effect->effectUsesTempoData())
	{
		float bpm(0.f);
		unsigned int position(0u);
		if (this->getBPMandPosition(bpm, position))
		{
			effect->setTempoData(bpm, position);
		}
	}

	this->pluginParameters->updateParameters();
	unsigned int sampleFrames = static_cast<unsigned int>(sampleFrames_);
	for (unsigned int blockOffset = 0; blockOffset < sampleFrames; blockOffset += constants::blockSize) {
		unsigned int blockLength = sampleFrames - blockOffset;
		// the last block of the buffer may be smaller than blockSize
		// we process it anyway, which might be source of jittering
		if (blockLength > constants::blockSize) {
			blockLength = constants::blockSize;
		}
		for (unsigned sampleOffset = 0; sampleOffset < blockLength; sampleOffset++) {
			unsigned int samplePosition = blockOffset + sampleOffset;
			processBuffer[sampleOffset] = Sample(inputs[0][samplePosition], inputs[1][samplePosition]);
		}
		onBeforeBlock(blockOffset);
		effect->processBlock(processBuffer, blockLength);
		for (unsigned sampleOffset = 0; sampleOffset < blockLength; sampleOffset++) {
			unsigned int samplePosition = blockOffset + sampleOffset;
			alignas(16) double outputSample[2];
			processBuffer[sampleOffset].store_aligned(outputSample);
			outputs[0][samplePosition] = float(outputSample[0]);
			outputs[1][samplePosition] = float(outputSample[1]);
		}
	}
	onAfterProcess();
}

void PluginBase::setParameter(VstInt32 index, float value)
{
	pluginParameters->setPluginParameter(index, value);
}	///< Called when a parameter changed

float PluginBase::getParameter(VstInt32 index) {
	return pluginParameters->getParameterNormalized(index);
}

void PluginBase::getParameterLabel(VstInt32 index, char* label)
{
	std::string unit = pluginParameters->getParameterUnit(index);
	vst_strncpy(label, unit.c_str(), std::min(unit.length(), size_t(kVstMaxParamStrLen)));
}	///< Stuff \e label with the units in which parameter \e index is displayed (i.e. "sec", "dB", "type", etc...). Limited to #kVstMaxParamStrLen.

void PluginBase::getParameterDisplay(VstInt32 index, char* text)
{
	float value = pluginParameters->getParameterUnnormalized(index);
	std::string valueAsString(std::to_string(value));
	vst_strncpy(text, valueAsString.c_str(), std::min(valueAsString.length(), size_t(kVstMaxParamStrLen)));
}	///< Stuff \e text with a string representation ("0.5", "-3", "PLATE", etc...) of the value of parameter \e index. Limited to #kVstMaxParamStrLen.

void PluginBase::getParameterName(VstInt32 index, char* text)
{
	std::string name = pluginParameters->getParameterName(index);
	vst_strncpy(text, name.c_str(), std::min(name.length(), size_t(kVstMaxParamStrLen)));
}    ///< Stuff \e text with the name ("Time", "Gain", "RoomType", etc...) of parameter \e index. Limited to #kVstMaxParamStrLen.

bool PluginBase::getEffectName(char* name)
{
	vst_strncpy(name, "BH2_base", kVstMaxEffectNameLen);
	return true;
}

bool PluginBase::getVendorString(char* text) {
	vst_strncpy(text, "rtificial", kVstMaxVendorStrLen);
	return true;
}

bool PluginBase::getProductString(char* text) {
	vst_strncpy(text, "blankenhain2", kVstMaxProductStrLen);
	return true;
}

VstInt32 PluginBase::getVendorVersion() {
	return 1000;
}

const PluginParameterBundle& PluginBase::getParameters() const
{
	return *pluginParameters;
}

bool PluginBase::getBPMandPosition(float& bpm, unsigned int& position)
{
	VstTimeInfo* ptr = this->getTimeInfo(kVstTempoValid);
	if (ptr->flags | kVstTempoValid)
		bpm = ptr->tempo;
	else
		return false;
	position = static_cast<unsigned int>(ptr->samplePos);
	return true;
};

void PluginBase::onBeforeBlock(unsigned int blockOffset)
{ }

void PluginBase::onAfterProcess()
{ }