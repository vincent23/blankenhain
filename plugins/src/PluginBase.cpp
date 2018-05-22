#include "PluginBase.h"
#include "EffectBase.h"
#include "PluginParameterBundle.h"
#include "Sample.h"
#include "ImguiEffectEditor.h"

#include <string>
#include <algorithm>


// via http://fogbugz.nuedge.net/default.asp?W159
//
// Archived here in case site goes offline
//
//Hosts
//Ableton Live
//
//Versions Tested : 8.2
//Platforms Tested : Windows x86, Mac PPC x86
//Formats Tested : AU & VST
//
//Latency Compensation : can be updated dynamically with ioChanged().
// NOTE BY DUSTIN 11.02.17: Ableton does not call getInitialDelay() after ioChanged was called().
// ioChanged() returns false.
//
//Keyboard Input : works fine with OS events(kEventRawKeyDown and WM_KEYDOWN).
//
//Stepped parameters : Doesn't call getParameterProperties(). If a parameter is modified quickly in succession, Live will assume it is continuous and ramp the automation. Otherwise it will make a step.
//
//updateDisplay() (or AU property change on kAudioUnitProperty_CurrentPreset or kAudioUnitProperty_PresentPreset) causes host to assume parameters have changed, stopping automation of the plug - in and lighting up the "back to arrangement" button.
//
//Removing parameters in a plug - in update will result in Live trying to set parameters with indexes out of range when loading old projects.
//
//getParameterDisplay() is called from audio thread.
//
//stops calling process if for some time silence(and no midi events) was outputted and gui is closed. (bad if you have for example a midi sequencing plugin).Workaround send CC's all the time to stay active
//
//Does not use setBypass().When an effect is deactivated it is "suspended" and bypassed by Live itself, not by the plug - in.

PluginBase::PluginBase(audioMasterCallback const& audioMaster, EffectBase* effect_, bool producesTailOutput)
	: AudioEffectX(audioMaster, 1, effect_->getNumberOfParameters())
	, speakerArr(nullptr)
	, effect(effect_)
	, pluginParameters(new PluginParameterBundle(effect_->getParameterBundle()))
	, timeSinceLastBPMandPositionUpdate(0u)
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
	if (effect != nullptr)
	{
		delete effect;
		effect = nullptr;
	}
	if (pluginParameters != nullptr)
	{
		delete pluginParameters;
		pluginParameters = nullptr;
	}
}



bool PluginBase::canParameterBeAutomated(VstInt32 index)
{
	return true;
}

bool PluginBase::getParameterProperties(VstInt32 index, VstParameterProperties* p)
{
	// Seems not to be used in Live 8.2
	// see also:
	// https://www.kvraudio.com/forum/viewtopic.php?f=33&t=375315
	unsigned int vstIndex = static_cast<unsigned int>(index);
	if (vstIndex < this->getParameters().getNumberOfParameters())
	{
		VstParameterProperties& prop = *p;
		prop.flags = kVstParameterSupportsDisplayIndex + kVstParameterSupportsDisplayCategory;

		// TODO
		// BoolParameter* boolParam = dynamic_cast<BoolParameter*>(this->getParameters().getParameterBundle().getParameter(index));
		// IntegerParameter* intParam = dynamic_cast<IntegerParameter*>(this->getParameters().getParameterBundle().getParameter(index));
		// if (boolParam)
		// 	prop.flags += kVstParameterIsSwitch;
		// else if (intParam)
		// 	prop.flags += kVstParameterUsesIntStep;


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
	unsigned int vstIndex = static_cast<unsigned int>(index);
	// Seems not to be used in Live 8.2

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

	if (vstIndex < this->getParameters().getNumberOfParameters())
	{
		float floatValueUnnormalized = static_cast<float>(atof(text));
		if (pluginParameters->getParameter(vstIndex)->getRange().isInRange(floatValueUnnormalized))
		{
			float floatValueNormalized = pluginParameters->getParameter(vstIndex)->getRange().toNormalized(floatValueUnnormalized);
			this->pluginParameters->setPluginParameter(vstIndex, floatValueNormalized);

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
	updateTempoData();

	this->pluginParameters->updateParameters();
	const unsigned int sampleFrames = static_cast<unsigned int>(sampleFrames_);
	for (unsigned int blockOffset = 0; blockOffset < sampleFrames; blockOffset += constants::blockSize) 
	{
		unsigned int blockLength = sampleFrames - blockOffset;
		// the last block of the buffer may be smaller than blockSize
		// we process it anyway, which might be source of jittering

		if (blockLength > constants::blockSize) 
		{
			blockLength = constants::blockSize;
		}
		for (unsigned sampleOffset = 0; sampleOffset < blockLength; sampleOffset++) 
		{
			unsigned int samplePosition = blockOffset + sampleOffset;
			processBuffer[sampleOffset] = Sample(inputs[0][samplePosition], inputs[1][samplePosition]);
		}
		onBeforeBlock(blockOffset);
		effect->processBlock(processBuffer, blockLength);
		for (unsigned sampleOffset = 0; sampleOffset < blockLength; sampleOffset++) 
		{
			unsigned int samplePosition = blockOffset + sampleOffset;
			alignas(16) floatType outputSample[2];
			processBuffer[sampleOffset].store_aligned(outputSample);
			outputs[0][samplePosition] = float(outputSample[0]);
			outputs[1][samplePosition] = float(outputSample[1]);
		}

		// increment tempodata.position
		incrementTempoDataPosition(blockLength);
	}
	onAfterProcess();

	if (delayOfEffect != effect->getDelay())
	{
		this->delayOfEffect = effect->getDelay();
		this->setInitialDelay(effect->getDelay());
		/*bool itWorked = */this->ioChanged();
	}
}

void PluginBase::setParameter(VstInt32 index, float value)
{
	pluginParameters->setPluginParameter(index, value);
}	///< Called when a parameter changed

float PluginBase::getParameter(VstInt32 index) 
{
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

EffectBase* PluginBase::getEffect()
{
	return this->effect;
}


bool PluginBase::getBPMandPositionFromHost(float& bpm, unsigned int& position)
{
	VstTimeInfo* ptr = this->getTimeInfo(kVstTempoValid);
	if (!ptr) {
		return false;
	}
	if (ptr->flags | kVstTempoValid)
	{
		bpm = static_cast<float>(ptr->tempo);
		position = static_cast<unsigned int>(ptr->samplePos);
		return true;
	}
	else
		return false;
};

void PluginBase::updateTempoData(bool force)
{
	if (!force && timeSinceLastBPMandPositionUpdate <= 10000u) {
		return;
	}
	if (effect->effectUsesTempoData())
	{
		float bpm(0.f);
		unsigned int position(0u);
		if (this->getBPMandPositionFromHost(bpm, position))
		{
			effect->setTempoData(bpm, position);
		}
		timeSinceLastBPMandPositionUpdate = 0u;
	}
}

void PluginBase::incrementTempoDataPosition(unsigned int increment)
{
	if (effect->effectUsesTempoData())
	{
		effect->incrementTempoDataPosition(increment);
		timeSinceLastBPMandPositionUpdate += increment;
	}
}

void PluginBase::onBeforeBlock(unsigned int blockOffset)
{ }

void PluginBase::onAfterProcess()
{ }