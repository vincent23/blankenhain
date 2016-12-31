#pragma once

#include "audioeffectx.h"
#include "PluginParameterBundle.h"
#include "..\..\libblankenhain\include\ParameterBundle.h"

class PluginParameterBundle;
class EffectBase;
struct Sample;


/**
 * Our VST(i) plugin base class
 *
 * Every plug-in, wether its an instrument or effect or whatever derives from this
 * We already provide some of the functions and handle some of the vst-calls in
 * a nice blankenhain fashion!
 *
 * List of functions a plugin MUST call itself:
 * - isSynth(bool)
 * - noTail(bool)
 *
 * List of functions a plugin MUST provide itself:
 * - VstInt32 processEvents(VstEvents* events) override;
 *    (only if Plugin handles MIDI-Events)
 *
 * - void setInitialDelay (VstInt32 delay)
 *    (only if your Plugin has delay due to CircularBuffers and such)
 *
 * - VstInt32 getTailSize()
 *
 *
 * List of functions a plugin may still provide itself:
 *
 */

class PluginBase : public AudioEffectX {
public:
	/*
	 * @param audioMaster: handed through by host
	 * @param effect: pointer to actuall bh2 effect from libblankenhain
	 * @param producesTailOutput: Set false if you are sure that this effect is silent when silence if passed through 
	 * (ie no delay line or release time and so on).
	 */
	PluginBase(audioMasterCallback const& audioMaster, EffectBase* effect, bool producesTailOutput = true);
	virtual ~PluginBase();

	/*
	 * Always true, all bh2 parameters can be automated
	 */
	bool canParameterBeAutomated(VstInt32 index) override;

	/*
	 * Tries to convert a string representation of a unnormalized parameter value
	 * and then sets parameter to this value.
	 */
	bool string2parameter(VstInt32 index, char* text) override;

	bool setSpeakerArrangement(VstSpeakerArrangement* pluginInput, VstSpeakerArrangement* pluginOutput) override;
	bool getSpeakerArrangement(VstSpeakerArrangement** pluginInput, VstSpeakerArrangement** pluginOutput) override;
	void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) override;
	void setParameter(VstInt32 index, float value) override;
	float getParameter(VstInt32 index) override;
	void getParameterLabel(VstInt32 index, char* label) override;
	void getParameterDisplay(VstInt32 index, char* text) override;
	void getParameterName(VstInt32 index, char* text) override;
	bool getEffectName(char* name) override;
	bool getVendorString(char* text) override;
	bool getProductString(char* text) override;
	VstInt32 getVendorVersion() override;
	bool getParameterProperties(VstInt32 index, VstParameterProperties* p);
	float getSampleRate() override;
	virtual VstPlugCategory getPlugCategory() 
	{
		return VstPlugCategory::kPlugCategEffect;
	}


	const PluginParameterBundle& getParameters() const;

	// TODO
	//virtual VstInt32 getChunk(void** data, bool isPreset = false) 
	//virtual VstInt32 setChunk(void* data, VstInt32 byteSize, bool isPreset = false)
	//virtual VstInt32 getProgram() { return curProgram; }					///< Return the index to the current program
	//virtual void setProgram(VstInt32 program) { curProgram = program; }	///< Set the current program to \e program
	//
	//virtual void setProgramName(char* name) {}				///< Stuff the name field of the current program with \e name. Limited to #kVstMaxProgNameLen.
	//virtual void getProgramName(char* name) { *name = 0; }	///< Stuff \e name with the name of the current program. Limited to #kVstMaxProgNameLen.
	//



protected:
	virtual void onBeforeBlock(unsigned int blockOffset);
	virtual void onAfterProcess();
	PluginParameterBundle* pluginParameters;
	EffectBase* effect;

private:
	VstSpeakerArrangement* speakerArr;

	Sample* processBuffer;
};