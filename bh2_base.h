#pragma once
#include "audioeffectx.h"
#include "FloatParameter.h"
#include <atomic>
#include "processFunctions.h"
#include <vector>
#include "Sample.h"
#include "AuxFunc.h"
#include <string>



class ParameterBundle
{
public:
  ParameterBundle(unsigned int numberOfParameters_) : numberOfParameters(numberOfParameters_)
  {
    ParameterWithProperties** stuff = new ParameterWithProperties*[numberOfParameters];
    parameters = stuff;
    bufferScalingValue = 0.f;
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

  ParameterWithProperties*& getValue(unsigned int const& in) const
  {
    return (parameters)[in];
  };

  float bufferScalingValue;
private:
  const unsigned int numberOfParameters;
  ParameterWithProperties** parameters;
};



template<size_t BlockSize, typename ProcessFunction>
void processBlockwise(float* const* audioBufferIn, float* const* audioBufferOut, size_t numberOfSamples,
  Sample* sseBuffer, size_t numberOfParameters, float* currentParameters, size_t interpolaionDistance, ProcessFunction processFunction) {
  // Main Loop, performed till AudioBufferFloats are less than an integer multiple of Blocksize
  size_t offset = interpolaionDistance;
  for (; static_cast<int>(offset + BlockSize) <= numberOfSamples; offset += BlockSize)
  {
    // This fills SIMD-Sample-Array of size "Blocksize" from AudioBuffer
    for (size_t i = 0; i < BlockSize; i++) {
      int sampleIndex = offset + i;
      sseBuffer[i] = Sample(audioBufferIn[0][sampleIndex], audioBufferIn[1][sampleIndex]);
    }

    //


    // Now, audio processing is performed
    processFunction(sseBuffer, BlockSize, offset, numberOfParameters, currentParameters);
    alignas(16) double sampleValues[2];
    // And data is written back from SIMD-Sample-Array to AudioBuffer
    for (size_t i = 0; i < BlockSize; i++) {
      int sampleIndex = offset + i;
      sseBuffer[i].store_aligned(sampleValues);
      audioBufferOut[0][sampleIndex] = sampleValues[0];
      audioBufferOut[1][sampleIndex] = sampleValues[1];
    }
  }

  // Take care of the remaining samples
  const size_t remainingSamples = numberOfSamples - offset;
  if (remainingSamples != 0u)
  {
    for (size_t i = 0; i < remainingSamples; i++) {
      int sampleIndex = offset + i;
      sseBuffer[i] = Sample(audioBufferIn[0][sampleIndex], audioBufferIn[1][sampleIndex]);
    }
    processFunction(sseBuffer, remainingSamples, offset, numberOfParameters, currentParameters);
    alignas(16) double sampleValues[2];
    for (size_t i = 0; i < remainingSamples; i++) {
      int sampleIndex = offset + i;
      sseBuffer[i].store_aligned(sampleValues);
      audioBufferOut[0][sampleIndex] = sampleValues[0];
      audioBufferOut[1][sampleIndex] = sampleValues[1];
    }
  }
}



class BH2_effect_base
{
public:

  BH2_effect_base() : blockSize(128), sseBuffer(nullptr), params(nullptr), currentParameters(nullptr)
  {
    sseBuffer = new Sample[128];
  }

  BH2_effect_base(unsigned int const& blockSize_) : blockSize(blockSize_) , sseBuffer(nullptr), params(nullptr), currentParameters(nullptr)
  {
    sseBuffer = new Sample[blockSize];
  }

  ~BH2_effect_base()
  {
    if (sseBuffer != nullptr) delete[] sseBuffer;
    sseBuffer = nullptr;
  }

  virtual void processBlock(float* const* inputs, float* const* outputs, unsigned int bufferSize)
  {

    bool willBeInterpolated = false;
    unsigned int interpolationDistance = 0u;

    for (unsigned int j = 0u; j < this->params->getNumberOfParameters(); j++)
    {
      if (!this->params->getParameter(j)->oldAndTargetValueMatch())
      {
        willBeInterpolated = true; break;
      }
    }

    if (willBeInterpolated)
    {

      interpolationDistance = blockSize < bufferSize ? blockSize : bufferSize;

      for (size_t i = 0; i < interpolationDistance; i++) {
        sseBuffer[i] = Sample(inputs[0][i], inputs[1][i]);
      }

      for (unsigned int j = 0u; j < this->params->getNumberOfParameters(); j++)
      {
        this->params->getParameter(j)->setInterpolationDistance(blockSize);
      }


      alignas(16) double interpolationTempStorage[2];
      for (unsigned int i = 0u; i < interpolationDistance; i++)
      {
        for (unsigned int j = 0u; j < this->params->getNumberOfParameters(); j++)
        {
          this->currentParameters[j] = this->params->getParameter(j)->getImmediateValueAndUpdateUnnormalized();
        }
        this->process(sseBuffer + i, 1u, this->params->getNumberOfParameters(),
          this->currentParameters);
        sseBuffer[i].store_aligned(interpolationTempStorage);
        outputs[0][i] = interpolationTempStorage[0];
        outputs[1][i] = interpolationTempStorage[1];
      }

    }

    if (bufferSize - interpolationDistance != 0u)
    {
      for (unsigned int j = 0u; j < this->params->getNumberOfParameters(); j++)
      {
        this->currentParameters[j] = this->params->getParameter(j)->getTargetValueUnnormalized();
      }

      processBlockwise<constants::blockSize>(inputs, outputs, bufferSize, this->sseBuffer,
        params->getNumberOfParameters(), currentParameters, interpolationDistance,
        [this](Sample* sseBuffer_, size_t samples, size_t offset, size_t numberOfParam, float* currentParam)
      {
        this->process(sseBuffer_, samples, params->getNumberOfParameters(), currentParameters);
      }
      );
    }

    //Set current values as old values for interpolation in next buffer iteration
    if (willBeInterpolated)
    {
      for (size_t i = 0u; i < this->params->getNumberOfParameters(); i++)
      {
        ParameterWithProperties* parameter = this->params->getParameter(i);
        if (bufferSize - interpolationDistance != 0u)
        {
          parameter->setOldValueNormalized(parameter->getTargetValueNormalized());
          parameter->setImmediateValueNormalized(parameter->getTargetValueNormalized());
        }
        else
        {
          parameter->setOldValueNormalized(parameter->getImmediateValueNormalized());
        }
        parameter->resetInterpolationCounter();
      }
    }
  }
  
  virtual void process(Sample* buffer, size_t sampleFrames, size_t numberOfParameters, float* parameters) = 0;

  ParameterBundle* getPointerToParameterBundle() const
  {
    return (this->params);
  }


protected:
  float* currentParameters;
  ParameterBundle* params;
private:
  size_t blockSize;
  Sample* sseBuffer;
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
      (parameterconversions)->getParameter(i)->setTargetValueNormalized(vst_parameters[i]);
    }
  };

  std::string getVSTParameterName(unsigned int const& in)
  {
    if (in > vst_parameters.size()) return "error argument too big";
    else return (parameterconversions)->getParameter(in)->getName();
  };

  float getParameterNormalized(unsigned int const& index)
  {
    return this->vst_parameters[index];
  }

  float getParameterUnnormalized(unsigned int const& index)
  {
    return this->parameterconversions->getParameter(index)->fromNormalized(this->vst_parameters[index]);
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

    speakerArr = nullptr;
    vstparameters = nullptr;
    bh_base = nullptr;

    //	virtual void setInitialDelay (VstInt32 delay);		///< Use to report the plug-in's latency (Group Delay)

  };

  ~BH2_base()
  {
    deallocateArrangement(&speakerArr);
  }

  virtual void open() = 0;

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
    float *const * c_inputs = inputs;
    float *const * c_outputs = inputs;

    this->vstparameters->updateParameters();
    bh_base->processBlock(c_inputs, c_outputs, sampleFrames);
  }

  virtual void setParameter(VstInt32 index, float value) 
  {
    vstparameters->setVSTParameter(index, value);
  }	///< Called when a parameter changed

  virtual float getParameter(VstInt32 index) {
    return vstparameters->getParameterNormalized(index);
  }

  virtual void getParameterLabel(VstInt32 index, char* label) 
  { 
    char* holder = vstparameters->getParameterUnit(index);
    strncpy(label, holder, 8);
  }	///< Stuff \e label with the units in which parameter \e index is displayed (i.e. "sec", "dB", "type", etc...). Limited to #kVstMaxParamStrLen.
  
  virtual void getParameterDisplay(VstInt32 index, char* text) 
  { 
    float temp = vstparameters->getParameterUnnormalized(index);
    std::string temp2 = std::string(std::to_string(temp));
    char* willBeReturned = new char[8u];
    strncpy(text, temp2.c_str(), 8);

  }	///< Stuff \e text with a string representation ("0.5", "-3", "PLATE", etc...) of the value of parameter \e index. Limited to #kVstMaxParamStrLen.
  
  virtual void getParameterName(VstInt32 index, char* text) 
  { 
    char* holder = vstparameters->getParameterNameCstr(index);
    strncpy(text, holder, 8);

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
