#pragma once

#include "audioeffectx.h"

class PluginParameterBundle;
class EffectBase;

class PluginBase : public AudioEffectX {
public:
	PluginBase(audioMasterCallback const& audioMaster, EffectBase* effect);
	virtual ~PluginBase();

	virtual void open() = 0;
	virtual void close() = 0;

	VstInt32 processEvents(VstEvents* events) override;
	bool canParameterBeAutomated(VstInt32 index) override;
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

	// TODO
	//virtual VstInt32 getChunk(void** data, bool isPreset = false) 
	//virtual VstInt32 setChunk(void* data, VstInt32 byteSize, bool isPreset = false)
	//virtual VstInt32 getProgram() { return curProgram; }					///< Return the index to the current program
	//virtual void setProgram(VstInt32 program) { curProgram = program; }	///< Set the current program to \e program
	//
	//virtual void setProgramName(char* name) {}				///< Stuff the name field of the current program with \e name. Limited to #kVstMaxProgNameLen.
	//virtual void getProgramName(char* name) { *name = 0; }	///< Stuff \e name with the name of the current program. Limited to #kVstMaxProgNameLen.
	//

	//bool getParameterProperties(VstInt32 index, VstParameterProperties* p) 
	//{ 
	//  //VstParameterProperties prop;
	//  //prop.stepFloat = 0.f;
	//  //prop.smallStepFloat = 0.f;
	//  //prop.largeStepFloat = 0.f;
	//  //std::string name = vstparameters->getVSTParameterName(index);
	//  //const char* cname = name.c_str();
	//  //if (name.length() < 64)
	//  //{
	//  //  delete[] prop.label;
	//  //  prop.label = new char[name.length];
	//  //
	//  //}
	//  //else
	//  //{
	//  //  char* tempcname = new char[64];
	//  //  strncpy(tempcname, cname, 64);
	//  //}
	//  //
	//  return false; 
	//} ///< Return parameter properties

private:
	VstSpeakerArrangement* speakerArr;
	PluginParameterBundle* pluginParameters;
	EffectBase* effect;
};