#ifdef PLUGIN_LIMITER

#include "LimiterAudioProcessor.h"
#include "LimiterAudioProcessorEditor.h"

LimiterAudioProcessor::LimiterAudioProcessor()
{
  lookahead = new FloatParameter(5.f, "lookahead", 1.f, NormalizedRange(0.f, 5000.f, 1.f)); //in samples
  release = new FloatParameter(10.f / 132.f, "release", 1.f, NormalizedRange(0.f, 1000.f, 1.f));
  threshold = new FloatParameter(0.f, "threshold", 1.f, NormalizedRange(-36.f, 0.f, 3.f));
  addParameter(lookahead);
  addParameter(release);
  addParameter(threshold);
}

void LimiterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
  longCircularBuffer = new CircularBuffer<float>(100u);
  shortCircularBuffer1 = new CircularBuffer<float>(100);
  shortCircularBuffer2 = new CircularBuffer<float>(100);
    // initialisation that you need..
}

void LimiterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
  delete[] longCircularBuffer;
  delete[] shortCircularBuffer1;
  delete[] shortCircularBuffer2;
}

void LimiterAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// I've added this to avoid people getting screaming feedback
	// when they first compile the plugin, but obviously you don't need to
	// this code if your algorithm already fills all the output channels.
	for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}

  if (longCircularBuffer->getSize() != lookahead->getValue())
  {
    longCircularBuffer->setSize(lookahead->getValue());
  }
  if (shortCircularBuffer1->getSize() != lookahead->getValue() / 2)
  {
    shortCircularBuffer1->setSize(lookahead->getValue() / 2);
  }
  if (shortCircularBuffer2->getSize() != lookahead->getValue() / 2)
  {
    shortCircularBuffer2->setSize(lookahead->getValue() / 2);
  }

  this->initializing(buffer);

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	jassert(getNumInputChannels() == 2);
	float release_ = release->getValue();
	float threshold_ = threshold->getValue();
	for (int i = 0; i < buffer.getNumSamples(); i++) {
		for (int channel = 0; channel < getNumInputChannels(); channel++) 
    {
      float current = longCircularBuffer->pushpop(buffer.getSample(channel, i));
      float max = longCircularBuffer->getMax();
      float gainReduction = 1.f / max;
      shortCircularBuffer1->push(gainReduction);
      float sCB1_normsum = shortCircularBuffer1->getSum() / shortCircularBuffer1->getSize();
      shortCircularBuffer2->push(sCB1_normsum);
      float sCB2_normsum = shortCircularBuffer2->getSum() / shortCircularBuffer2->getSize();
      if (sCB2_normsum < gainReduction * release_ / 1000.f) gainReduction = sCB2_normsum;
      if (buffer.getSample(channel, i) > pow(10.f, threshold_ / 10.f))
      {
        buffer.setSample(channel, i, current * gainReduction);
      }
		}
	}

  this->meteringBuffer(buffer);
  this->finalizing(buffer);
}

AudioProcessorEditor* LimiterAudioProcessor::createEditor()
{
	return new LimiterAudioProcessorEditor(*this);
}

void LimiterAudioProcessor::setRelease(FloatParameter release_) {
  release->setValueNotifyingHost(release_.getNormalizedValue());
}

void LimiterAudioProcessor::setLookahead(FloatParameter newLookahead) 
{
  lookahead->setValueNotifyingHost(newLookahead.getNormalizedValue());
}

void LimiterAudioProcessor::setThreshold(FloatParameter newThreshold) {
  threshold->setValueNotifyingHost(newThreshold.getNormalizedValue());
}

void LimiterAudioProcessor::setState(const var & state)
{
  lookahead->setValue(state.getProperty("lookahead", lookahead->getValue()));
  release->setValue(state.getProperty("release", release->getValue()));
  threshold->setValue(state.getProperty("threshold", threshold->getValue()));
}

var LimiterAudioProcessor::getState()
{
  DynamicObject* properties = new DynamicObject;
  properties->setProperty("lookahead", lookahead->getValue());
  properties->setProperty("release", release->getValue());
  properties->setProperty("threshold", threshold->getValue());
  return var(&properties);
}

float LimiterAudioProcessor::getRelease() {
  return release->getValue();
}

float LimiterAudioProcessor::getLookahead() {
  return lookahead->getValue();
}

float LimiterAudioProcessor::getThreshold() {
  return threshold->getValue();
}

#endif