#ifndef BLANKENHAIN_AUDIO_PROCESSOR_H_INCLUDED
#define BLANKENHAIN_AUDIO_PROCESSOR_H_INCLUDED

#include <juce>
#include <vector>

class BlankenhainAudioProcessor : public AudioProcessor
{
public:
	BlankenhainAudioProcessor();

	bool hasEditor() const override;

	const String getName() const override;

	const String getInputChannelName(int channelIndex) const override;
	const String getOutputChannelName(int channelIndex) const override;
	bool isInputChannelStereoPair(int index) const override;
	bool isOutputChannelStereoPair(int index) const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool silenceInProducesSilenceOut() const override;
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

protected:
	virtual var getState() = 0;
	virtual void setState(const var& state) = 0;

	double lastKnownSampleRate = 44100;
	int lastKnownBlockSize = 512;

	//Left Peak, Right Peak, Left RMS, Right RMS
	std::vector<float> meterValues = { 0.f,0.f,0.f,0.f };

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlankenhainAudioProcessor)
};

#endif  // BLANKENHAIN_AUDIO_PROCESSOR_H_INCLUDED
