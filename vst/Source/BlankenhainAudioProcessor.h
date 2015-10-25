#ifndef BLANKENHAIN_AUDIO_PROCESSOR_H_INCLUDED
#define BLANKENHAIN_AUDIO_PROCESSOR_H_INCLUDED

#include <juce>

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

protected:
	virtual var getState() = 0;
	virtual void setState(const var& state) = 0;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlankenhainAudioProcessor)
};

#endif  // BLANKENHAIN_AUDIO_PROCESSOR_H_INCLUDED
