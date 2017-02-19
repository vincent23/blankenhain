#pragma once

#include "audioeffectx.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "EffectBase.h"

#include "Constants.h"

class PluginParameterBundle;
class Sample;


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
 * - onBeforeBlock(unsigned int blockOffset) -> only if plugin is not an instrument;
 * - onAfterProcess();
 *
 */

class PluginBase : public AudioEffectX, public AlignedType {
public:
	/*
	 * @param audioMaster: handed through by host
	 * @param effect: pointer to actuall bh2 effect from libblankenhain
	 * @param producesTailOutput: Set false if you are sure that this effect is silent when silence if passed through
	 * (ie no delay line or release time and so on).
	 */
	PluginBase(audioMasterCallback const& audioMaster, EffectBase* effect, bool producesTailOutput = true);
	virtual ~PluginBase();

	/////////////////////////////////////////////////////
	//// OVERRIDES FOR VST SDK FUNCTIONS
	//// WE HANDLE OUR OWN SHIT
	/////////////////////////////////////////////////////

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
	bool getParameterProperties(VstInt32 index, VstParameterProperties* p) override;
	float getSampleRate() override;
	VstPlugCategory getPlugCategory() override
	{
		return VstPlugCategory::kPlugCategEffect;
	}

	void open() override
	{
		updateTempoData(true);
	}

	virtual VstInt32 startProcess() override
	{
		// this is actually called when first time loading a plugin
		updateTempoData(true);
		return 0;
	}

	virtual VstInt32 stopProcess() override
	{
		// this is actually called when deleting a plugin
		return 0u;
	}

	void resume() override
	{
		updateTempoData(true);
	}


	/////////////////////////////////////////////////////
	//// RTIFICIAL FLAVOR
	//// ACCESS THE EFFECT!
	/////////////////////////////////////////////////////


	const PluginParameterBundle& getParameters() const;

	// TODO maybe
	//virtual VstInt32 getChunk(void** data, bool isPreset = false) 
	//virtual VstInt32 setChunk(void* data, VstInt32 byteSize, bool isPreset = false)
	//virtual VstInt32 getProgram() { return curProgram; }					///< Return the index to the current program
	//virtual void setProgram(VstInt32 program) { curProgram = program; }	///< Set the current program to \e program
	//
	//virtual void setProgramName(char* name) {}				///< Stuff the name field of the current program with \e name. Limited to #kVstMaxProgNameLen.
	//virtual void getProgramName(char* name) { *name = 0; }	///< Stuff \e name with the name of the current program. Limited to #kVstMaxProgNameLen.
	//



protected:
	/*
	* We don't use getTimeInfo directly because we dont need all
	* the information but only bpm and position in samples.
	*
	* Returns true when tempo is valid. Only use the values you get
	* in this function when it returns true!
	*/
	bool getBPMandPositionFromHost(float& bpm, unsigned int& position);

	void updateTempoData(bool force = false);

	void incrementTempoDataPosition(unsigned int increment);

	/*
	 * This function may be provided optionaly. It
	 * is called pretty much right before the call to effect->process()
	 */
	virtual void onBeforeBlock(unsigned int blockOffset);

	/*
	 * This function may be provided optionaly. It
	 * is called at the end of processReplacing and therefore after the call to effect->process()
	 */
	virtual void onAfterProcess();

	EffectBase* effect;

	/// this used to be private, but since MidiEffects get their own,
	/// lean and mean processReplacing function, they need to be able to access this
	PluginParameterBundle* pluginParameters;

private:
	unsigned int timeSinceLastBPMandPositionUpdate;
	VstSpeakerArrangement* speakerArr;
	Sample processBuffer[constants::blockSize];
};