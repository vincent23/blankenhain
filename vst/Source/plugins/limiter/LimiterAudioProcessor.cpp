#ifdef PLUGIN_LIMITER

#include "LimiterAudioProcessor.h"
#include "LimiterAudioProcessorEditor.h"

LimiterAudioProcessor::LimiterAudioProcessor()
{
  lookahead = new FloatParameter(5.f, "lookahead", 1.f, NormalizedRange(0.f, 5000.f, 1.f)); //in samples
  release = new FloatParameter(10.f / 132.f, "release", 1.f, NormalizedRange(0.f, 1000.f, 1.f));
  ogain = new FloatParameter(0.5f, "ogain", 1.f, NormalizedRange(-5.f, 5.f, 1.f));
  igain = new FloatParameter(0.5f, "igain", 1.f, NormalizedRange(-5.f, 15.f, 1.f));
  bypass = new BoolParameter("bypass", false);
  addParameter(lookahead);
  addParameter(ogain);
  addParameter(igain);
  addParameter(bypass);
  addParameter(release);
  suggestedOgain = 99999999999.f;
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
  if (longCircularBuffer != nullptr) 
  {
  delete longCircularBuffer;
  longCircularBuffer = nullptr;
  }
  if (shortCircularBuffer1 != nullptr)
  {
    delete shortCircularBuffer1;
    shortCircularBuffer1 = nullptr;
  }
  if (shortCircularBuffer2 != nullptr)
  {
    delete shortCircularBuffer2;
    shortCircularBuffer2 = nullptr;
  }
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

  for (int i = 0; i < buffer.getNumSamples(); i++) {
    for (int channel = 0; channel < getNumInputChannels(); channel++)
    {
      if (buffer.getSample(channel, i) > 1.f)
      {
        float in = buffer.getSample(channel, i);
        std::cout << in;
      }
    }
  }

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	jassert(getNumInputChannels() == 2);
  if (!bypass->getBoolValue())
  {
    float release_ = release->getValue();
    for (int i = 0; i < buffer.getNumSamples(); i++) {
      for (int channel = 0; channel < getNumInputChannels(); channel++)
      {
        float current = longCircularBuffer->pushpop(buffer.getSample(channel, i) * igain->getValue());
        float max = longCircularBuffer->getAbsMax() > (current > 0.f ? current : -1.f * current) ? longCircularBuffer->getAbsMax() : (current > 0.f ? current : -1.f * current);
        float gainReduction = max > 1.f ? 1.f / max : 1.f;
        shortCircularBuffer1->push(gainReduction);
        float sCB1_normsum = shortCircularBuffer1->getSum() / shortCircularBuffer1->getSize();
        shortCircularBuffer2->push(sCB1_normsum);
        float sCB2_normsum = shortCircularBuffer2->getSum() / shortCircularBuffer2->getSize();
        if (sCB2_normsum > gainReduction * release_ / 500.f)
        {
          gainReduction = sCB2_normsum;
        }
        float ogainnow = ogain->getValue();
        float olducrrent = current;
        if (suggestedOgain > 1.f / std::abs(current * gainReduction)) suggestedOgain = 1.f / std::abs(current * gainReduction);
        current *= ogain->getValue();
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

void LimiterAudioProcessor::setOgain(FloatParameter newOgain)
{
  ogain->setValueNotifyingHost(newOgain.getNormalizedValue());
}

void LimiterAudioProcessor::setIgain(FloatParameter newIgain)
{
  igain->setValueNotifyingHost(newIgain.getNormalizedValue());
}

void LimiterAudioProcessor::switchBypass() {
  bypass->setValueNotifyingHost((float)(!bypass->getBoolValue()));
}

void LimiterAudioProcessor::setState(const var & state)
{
  lookahead->setValue(state.getProperty("lookahead", lookahead->getValue()));
  release->setValue(state.getProperty("release", release->getValue()));
  ogain->setValue(state.getProperty("ogain", ogain->getValue()));
  igain->setValue(state.getProperty("igain", igain->getValue()));
  bypass->setValue(state.getProperty("bypass", bypass->getBoolValue()));
}

var LimiterAudioProcessor::getState()
{
  DynamicObject* properties = new DynamicObject;
  properties->setProperty("lookahead", lookahead->getValue());
  properties->setProperty("release", release->getValue());
  properties->setProperty("ogain", ogain->getValue());
  properties->setProperty("igain", igain->getValue());
  properties->setProperty("bypass", bypass->getBoolValue());
  return var(&properties);
}

float LimiterAudioProcessor::getRelease() {
  return release->getValue();
}

float LimiterAudioProcessor::getLookahead() {
  return lookahead->getValue();
}

float LimiterAudioProcessor::getOgain() {
  return ogain->getValue();
}

float LimiterAudioProcessor::getIgain() {
  return igain->getValue();
}

bool LimiterAudioProcessor::getBypass() {
  return bypass->getBoolValue();
}

#endif