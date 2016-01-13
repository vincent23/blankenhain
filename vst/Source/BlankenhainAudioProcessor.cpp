#include "BlankenhainAudioProcessor.h"

BlankenhainAudioProcessor::BlankenhainAudioProcessor()
{
	busArrangement.inputBuses.clear();
	busArrangement.outputBuses.clear();
	busArrangement.inputBuses.add(AudioProcessorBus("Input", AudioChannelSet::stereo()));
	busArrangement.outputBuses.add(AudioProcessorBus("Output", AudioChannelSet::stereo()));
}

const String BlankenhainAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

const String BlankenhainAudioProcessor::getInputChannelName(int channelIndex) const
{
	return String(channelIndex + 1);
}

const String BlankenhainAudioProcessor::getOutputChannelName(int channelIndex) const
{
	return String(channelIndex + 1);
}

bool BlankenhainAudioProcessor::isInputChannelStereoPair(int index) const
{
	return true;
}

bool BlankenhainAudioProcessor::isOutputChannelStereoPair(int index) const
{
	return true;
}

bool BlankenhainAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool BlankenhainAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool BlankenhainAudioProcessor::silenceInProducesSilenceOut() const
{
	return false;
}

double BlankenhainAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int BlankenhainAudioProcessor::getNumPrograms()
{
	// NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
	return 1;
}

int BlankenhainAudioProcessor::getCurrentProgram()
{
	return 0;
}

void BlankenhainAudioProcessor::setCurrentProgram(int index)
{
}

const String BlankenhainAudioProcessor::getProgramName(int index)
{
	return String();
}

void BlankenhainAudioProcessor::changeProgramName(int index, const String& newName)
{
}

bool BlankenhainAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

void BlankenhainAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
	var state = getState();
	MemoryOutputStream outputStream(destData, false);
	JSON::writeToStream(outputStream, state, false);
}

void BlankenhainAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
	MemoryInputStream inputStream(data, sizeInBytes, false);
	var state = JSON::parse(inputStream);
	setState(state);
}

void BlankenhainAudioProcessor::setLastKnownSampleRate(double in)
{
	lastKnownSampleRate = in;
}

double BlankenhainAudioProcessor::getLastKnownSampleRate(void) 
{
	return this->lastKnownSampleRate;
}

void BlankenhainAudioProcessor::setLastKnownBlockSize(int in)
{
	lastKnownBlockSize = in;
}

int BlankenhainAudioProcessor::getLastKnownBlockSize(void)
{
	return this->lastKnownBlockSize;
}

std::vector<float> BlankenhainAudioProcessor::getMeterValues(void)
{
	return this->meterValues;
}

void BlankenhainAudioProcessor::initializing(AudioSampleBuffer& buffer)
{
	//For Metering, reset "current" Values
	for (unsigned int i = 0; i < meterValues.size(); i++)
	{
		meterValues[i] = 0.f;
	}

	//Set lastKnownSampleRate and lastKnownBlockSize
	this->setLastKnownSampleRate(this->getSampleRate());
	this->setLastKnownBlockSize(this->getBlockSize());

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data.
	for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}
}

void BlankenhainAudioProcessor::finalizing(AudioSampleBuffer& buffer)
{
	//Get RMSD
	meterValues[2] /= float(buffer.getNumSamples());
	meterValues[3] /= float(buffer.getNumSamples());
	meterValues[2] = sqrt(meterValues[2]);
	meterValues[3] = sqrt(meterValues[3]);
	if (meterValues[2] > 1 || meterValues[3] > 1)
	{

	}
}

void BlankenhainAudioProcessor::meteringBuffer(AudioSampleBuffer& buffer)
{
	/* METERING CODE */
	for (size_t iteration = 0; iteration < buffer.getNumSamples(); iteration++)
	{
		if (abs(buffer.getSample(/*channel*/ 0, iteration)) > meterValues[0])
		{
			meterValues[0] = abs(buffer.getSample(/*channel*/ 0, iteration));
		}
		if (abs(buffer.getSample(/*channel*/ 1, iteration)) > meterValues[1])
		{
			meterValues[1] = abs(buffer.getSample(/*channel*/ 1, iteration));
		}
		meterValues[2] += buffer.getSample(/*channel*/ 0, iteration) * buffer.getSample(/*channel*/ 0, iteration);
		meterValues[3] += buffer.getSample(/*channel*/ 1, iteration) * buffer.getSample(/*channel*/ 1, iteration);
	}
	/* END METERING CODE*/
}

void BlankenhainAudioProcessor::meteringSingle(float one, float two)
{
	/* METERING CODE */

	//Current
	if (abs(one) > meterValues[0])
	{
		meterValues[0] = abs(one);
	}
	if (abs(two) > meterValues[1])
	{
		meterValues[1] = abs(two);
	}
	meterValues[2] += one * one;
	meterValues[3] += two * two;

	/* END METERING CODE*/
}