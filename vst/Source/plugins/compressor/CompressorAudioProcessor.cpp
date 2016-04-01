#ifdef PLUGIN_COMPRESSOR

#include "CompressorAudioProcessor.h"
#include "CompressorAudioProcessorEditor.h"

CompressorAudioProcessor::CompressorAudioProcessor()
{
  ratio = new FloatParameter(1.f, "ratio", 1.f, NormalizedRange(0.f, 12.f, 1.f));
  release = new FloatParameter(10.f / 132.f, "release", 1.f, NormalizedRange(0.f, 1000.f, 1.f));
  threshold = new FloatParameter(0.f, "threshold", 1.f, NormalizedRange(-36.f, 3.f, 1.f));
  attack = new FloatParameter(10.f, "attack", 1.f, NormalizedRange(0.f, 1000.f, 1.f));
  postgain = new FloatParameter(0.f, "postgain", 1.f, NormalizedRange(0.f, 24.f, 1.f));
  addParameter(ratio);
  addParameter(release);
  addParameter(threshold);
  addParameter(attack);
  addParameter(postgain);
}

void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void CompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void CompressorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
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

  this->initializing(buffer);
  if (!this->getBypass())
  {
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    jassert(getNumInputChannels() == 2);
    float attack_ = attack->getValue() / 1000.f;
    float release_ = release->getValue() / 1000.f;
    float attackGain = exp(-1 / (attack_ * 44100));
    float releaseGain = exp(-1 / (release_ * 44100));
    float threshold_ = threshold->getValue();
    float ratio_ = ratio->getValue();
    float slope = 1 - (1 / ratio_);
    for (int i = 0; i < buffer.getNumSamples(); i++) {
      for (int channel = 0; channel < getNumInputChannels(); channel++) {
        float* channelData = buffer.getWritePointer(channel);
        float input = abs(channelData[i]);
        if (envelope[channel] < input) {
          envelope[channel] = input + attackGain * (envelope[channel] - input);
        }
        else {
          envelope[channel] = input + releaseGain * (envelope[channel] - input);
        }
      }
      float envelopeValue = std::max(envelope[0], envelope[1]);
      for (int channel = 0; channel < getNumInputChannels(); channel++) {
        float* channelData = buffer.getWritePointer(channel);
        float input = envelopeValue;
        float envelopeDb = aux::linearToDecibel(abs(input));
        if (envelopeDb < threshold_)
        {
          envelopeDb = 0.f;
        }
        else
        {
          envelopeDb = slope * (threshold_ - envelopeDb);
        }
        channelData[i] *= aux::decibelToLinear(envelopeDb);
        channelData[i] *= aux::decibelToLinear(postgain->getValue());
      }
    }
  }
  this->finalizing(buffer);
  this->meteringBuffer(buffer);
}

AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
	return new CompressorAudioProcessorEditor(*this);
}

void CompressorAudioProcessor::setRelease(float release_) {
  release->setValueNotifyingHost(release_);
}

void CompressorAudioProcessor::setRatio(float newRatio) {
  ratio->setValueNotifyingHost(newRatio);
}

void CompressorAudioProcessor::setAttack(float newAttack) {
  attack->setValueNotifyingHost(newAttack);
}

void CompressorAudioProcessor::setThreshold(float newThreshold) {
  threshold->setValueNotifyingHost(newThreshold);
}

void CompressorAudioProcessor::setPostgain(float newPostgain) {
  postgain->setValueNotifyingHost(newPostgain);
}

void CompressorAudioProcessor::setState(const var & state)
{
  ratio->setValue(state.getProperty("ratio", ratio->getValue()));
  attack->setValue(state.getProperty("attack", attack->getValue()));
  release->setValue(state.getProperty("release", release->getValue()));
  threshold->setValue(state.getProperty("threshold", threshold->getValue()));
  postgain->setValue(state.getProperty("postgain", postgain->getValue()));
}

var CompressorAudioProcessor::getState()
{
  DynamicObject* properties = new DynamicObject;
  properties->setProperty("ratio", ratio->getValue());
  properties->setProperty("attack", attack->getValue());
  properties->setProperty("release", release->getValue());
  properties->setProperty("threshold", threshold->getValue());
  properties->setProperty("postgain", postgain->getValue());
  return var(&properties);
}

float CompressorAudioProcessor::getRelease() {
  return release->getValue();
}

float CompressorAudioProcessor::getRatio() {
  return ratio->getValue();
}

float CompressorAudioProcessor::getAttack() {
  return attack->getValue();
}

float CompressorAudioProcessor::getThreshold() {
  return threshold->getValue();
}

float CompressorAudioProcessor::getPostgain() {
  return postgain->getValue();
}

#endif
