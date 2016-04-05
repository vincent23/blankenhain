#pragma once
#pragma warning( disable : 4100)
#include "AuxFunc.h"
#include <juce>
#include <vector>

// Range checks and so on. Good For VST,
// however takes CPU power.
// #define BLANKENHAIN_CHECKS
// define it in juice


class BlankenhainAudioProcessor : public AudioProcessor
{
public:
	BlankenhainAudioProcessor();

	bool hasEditor() const override;

	const String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	double getTailLengthSeconds() const override;

	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;

	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	double getLastKnownSampleRate(void);
	void setLastKnownSampleRate(double in);

	int getLastKnownBlockSize(void);
	void setLastKnownBlockSize(int in);

	std::vector<float> getMeterValues(void);

	//ONLY CALL THESE TWO FROM WITHIN PROCESSBLOCK
	void initializing(AudioSampleBuffer&);
	void finalizing(AudioSampleBuffer&);

	//METERING
	void meteringBuffer(AudioSampleBuffer&);
	void meteringSingle(float, float);
  bool getBypass() const;
  void setBypass(bool value);
  void setIngain(float value);
  float getIngain() const;
  float getOutgain() const;
  void setOutgain(float value);

protected:
	virtual var getState() = 0;
	virtual void setState(const var& state) = 0;

	template<size_t BlockSize, typename ProcessFunction>
	void processBlockwise(AudioSampleBuffer& audioBuffer, Sample* processBuffer, ProcessFunction processFunction);

  Sample* internalBuffer = new Sample[constants::blockSize];

	double lastKnownSampleRate = 44100;
	int lastKnownBlockSize = 512;

	//Left Peak, Right Peak, Left RMS, Right RMS
	std::vector<float> meterValues = { 0.f,0.f,0.f,0.f };
  bool bypass = false;
  float inGain = 1.f;
  float outGain = 1.f;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlankenhainAudioProcessor)
};


template<size_t BlockSize, typename ProcessFunction>
void BlankenhainAudioProcessor::processBlockwise(AudioSampleBuffer& audioBuffer, Sample* processBuffer, ProcessFunction processFunction) {
  // Main Loop, performed till AudioBufferFloats are less than an integer multiple of Blocksize
	size_t offset = 0;
  for (; static_cast<int>(offset + BlockSize) <= audioBuffer.getNumSamples(); offset += BlockSize) 
  {
    // This fills SIMD-Sample-Array of size "Blocksize" from AudioBuffer
		for (size_t i = 0; i < BlockSize; i++) {
			int sampleIndex = offset + i;
			processBuffer[i] = Sample(audioBuffer.getSample(0, sampleIndex), audioBuffer.getSample(1, sampleIndex));
		}
    // Now, audio processing is performed
		processFunction(BlockSize, offset);

    // And data is written back from SIMD-Sample-Array to AudioBuffer
		for (size_t i = 0; i < BlockSize; i++) {
			int sampleIndex = offset + i;
			alignas(16) double sampleValues[2];
			processBuffer[i].store_aligned(sampleValues);
			audioBuffer.setSample(0, sampleIndex, static_cast<float>(sampleValues[0]));
			audioBuffer.setSample(1, sampleIndex, static_cast<float>(sampleValues[1]));
		}
	}

  // Take care of the remaining samples
	const size_t remainingSamples = audioBuffer.getNumSamples() - offset;
  if (remainingSamples != 0u)
  {
    for (size_t i = 0; i < remainingSamples; i++) {
      int sampleIndex = offset + i;
      processBuffer[i] = Sample(audioBuffer.getSample(0, sampleIndex), audioBuffer.getSample(1, sampleIndex));
    }
    processFunction(remainingSamples, offset);
    for (size_t i = 0; i < remainingSamples; i++) {
      int sampleIndex = offset + i;
      alignas(16) double sampleValues[2];
      processBuffer[i].store_aligned(sampleValues);
      audioBuffer.setSample(0, sampleIndex, static_cast<float>(sampleValues[0]));
      audioBuffer.setSample(1, sampleIndex, static_cast<float>(sampleValues[1]));
    }
  }
}

