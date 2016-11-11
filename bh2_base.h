#pragma once
#include "audioeffectx.h"
#include "FloatParameter.h"
#include <atomic>
#include <vector>
#include <string>



class ParameterBundle
{
public:
  ParameterBundle(unsigned int numberOfParameters_) : numberOfParameters(numberOfParameters_)
  {
    ParameterWithProperties** stuff = new ParameterWithProperties*[numberOfParameters];
    parameters = stuff;
  }
  ~ParameterBundle()
  {
    if (parameters != nullptr)
    {
      delete[] * parameters;
      *parameters = nullptr;
    }
  }
  ParameterBundle(const ParameterBundle& other) :
    parameters(other.parameters), numberOfParameters(other.numberOfParameters)
  { }
  ParameterBundle(ParameterBundle&& other) noexcept : /* noexcept needed to enable optimizations in containers */
    parameters(other.parameters), numberOfParameters(other.numberOfParameters)
  { };
  ParameterBundle& operator= (const ParameterBundle& other)
  {
    ParameterBundle tmp(other);         // re-use copy-constructor
    *this = std::move(tmp); // re-use move-assignment
    return *this;
  }

  const unsigned int getNumberOfParameters() const
  {
    return numberOfParameters;
  }
  
  ParameterWithProperties*& getParameter(unsigned int const& in) const
  {
      return (parameters)[in];
  };
private:
  const unsigned int numberOfParameters;
  ParameterWithProperties** parameters;
};

class BH2_effect_base
{
public:
  virtual void process(float** inputs, float** outputs, unsigned int sampleFrames) = 0;
  ParameterBundle* getPointerToParameterBundle() const
  {
    return (this->params);
  }
protected:
  ParameterBundle* params;
  float bufferScalingValue = 0.05;
};

class VSTParameterBundle
{
public:
  VSTParameterBundle(unsigned int numberOfParameters, ParameterBundle* params)
    : vst_parameters(std::vector<std::atomic<float>>(numberOfParameters))
  {
    parameterconversions = params;
    for (auto& i : vst_parameters)
    {
      i = 0.f;
    }
  };

  void setVSTParameter(unsigned int const& in, float const& unnormalizedValue)
  {
    if (in >= vst_parameters.size()) return;
    vst_parameters[in] = unnormalizedValue;
  }

  void updateParameters() 
  {
    for (unsigned int i = 0; i < vst_parameters.size(); i++)
    {
      (parameterconversions)->getParameter(i)->setCurrentValueNormalized(vst_parameters[i]);
    }
  };

  std::string getVSTParameterName(unsigned int const& in)
  {
    if (in > vst_parameters.size()) return "error argument too big";
    else return (parameterconversions)->getParameter(in)->getName();
  };

  float getParameter(unsigned int const& index)
  {
    return (parameterconversions)->getParameter(index)->getCurrentValueNormalized();
  }

  char* getParameterUnit(unsigned int const& index)
  {
    std::string unit = (parameterconversions)->getParameter(index)->getUnit();
    char* willBeReturned = new char[8u];
    strncpy(willBeReturned, unit.c_str(), 8);
    return willBeReturned;
  }

  char* getParameterNameCstr(unsigned int const& index)
  {
    std::string unit = (parameterconversions)->getParameter(index)->getName(8);
    char* willBeReturned = new char[8u];
    strncpy(willBeReturned, unit.c_str(), 8);
    return willBeReturned;
  }
private:
  std::vector<std::atomic<float>> vst_parameters;
  ParameterBundle* parameterconversions;
};



class BH2_base : public AudioEffectX {
public:

  BH2_base(audioMasterCallback const& audioMaster, unsigned int numberOfParameters)
    : AudioEffectX(audioMaster, 1, numberOfParameters), speakerArr(nullptr)
  {
    canDoubleReplacing(false);
    setNumInputs(2);
    setNumOutputs(2);
    allocateArrangement(&speakerArr, 2);
    speakerArr->type = kSpeakerArrStereo;
    speakerArr->numChannels = 2;
    canProcessReplacing();
    noTail(false);

    //for now
    programsAreChunks(false);

    //	virtual void setInitialDelay (VstInt32 delay);		///< Use to report the plug-in's latency (Group Delay)

  };

  ~BH2_base()
  {
    deallocateArrangement(&speakerArr);
  }

  virtual void open() = 0;
  //{
  //  setUniqueID(856);
  //  isSynth(false);
  //}

  virtual void close() = 0;

  virtual VstInt32 processEvents(VstEvents* events)
  {
    return -5;
  };

  bool canParameterBeAutomated(VstInt32 index) 
  { 
    return true; 
  }

  bool string2parameter(VstInt32 index, char* text) 
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
    if (text == nullptr) return true;
    else
    {
      std::string strng(text);
      if (strng == vstparameters->getVSTParameterName(index))
        return true;
      else
        return false;
    }
    return false; 
  }

  bool setSpeakerArrangement(VstSpeakerArrangement* pluginInput, VstSpeakerArrangement* pluginOutput) 
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

  bool getSpeakerArrangement(VstSpeakerArrangement** pluginInput, VstSpeakerArrangement** pluginOutput) 
  { 
    pluginInput = &speakerArr;
    pluginOutput = &speakerArr;
    return true;
  } ///< Return the plug-in's speaker arrangements

  void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
  {
    bh_base->process(inputs, outputs, sampleFrames);
    this->vstparameters->updateParameters();
  }

  virtual void setParameter(VstInt32 index, float value) 
  {
    vstparameters->setVSTParameter(index, value);
  }	///< Called when a parameter changed

  virtual float getParameter(VstInt32 index) {
    return vstparameters->getParameter(index);
  }

  virtual void getParameterLabel(VstInt32 index, char* label) 
  { 
    label = vstparameters->getParameterUnit(index);
  }	///< Stuff \e label with the units in which parameter \e index is displayed (i.e. "sec", "dB", "type", etc...). Limited to #kVstMaxParamStrLen.
  
  virtual void getParameterDisplay(VstInt32 index, char* text) 
  { 
    float temp = vstparameters->getParameter(index);
    std::string temp2 = std::string(std::to_string(temp));
    char* willBeReturned = new char[8u];
    strncpy(willBeReturned, temp2.c_str(), 8);
    text = willBeReturned;

  }	///< Stuff \e text with a string representation ("0.5", "-3", "PLATE", etc...) of the value of parameter \e index. Limited to #kVstMaxParamStrLen.
  
  virtual void getParameterName(VstInt32 index, char* text) 
  { 
    text = vstparameters->getParameterNameCstr(index);

  }    ///< Stuff \e text with the name ("Time", "Gain", "RoomType", etc...) of parameter \e index. Limited to #kVstMaxParamStrLen.

  bool getEffectName(char* name) 
  {
    vst_strncpy(name, "BH2_base", kVstMaxEffectNameLen);
    return true;
  }

  bool getVendorString(char* text) {
    vst_strncpy(text, "Dustin Software", kVstMaxVendorStrLen);
    return true;
  }

  bool getProductString(char* text) {
    vst_strncpy(text, "BH2_base", kVstMaxProductStrLen);
    return true;
  }

  VstInt32 getVendorVersion() {
    return 1000;
  }

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

protected:

  VstSpeakerArrangement* speakerArr;

  VSTParameterBundle* vstparameters;
  BH2_effect_base* bh_base;
};