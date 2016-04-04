#ifdef PLUGIN_COMPRESSOR

#include "CompressorAudioProcessor.h"
#include "CompressorAudioProcessorEditor.h"

CompressorAudioProcessor::CompressorAudioProcessor()
  : delayLine(44100)
{
  ratio = new FloatParameter(1.f, "ratio", 1.f, NormalizedRange(0.f, 12.f, 1.f));
  release = new FloatParameter(10.f / 132.f, "release", 1.f, NormalizedRange(0.f, 100.f, 1.f));
  threshold = new FloatParameter(0.f, "threshold", 1.f, NormalizedRange(-48.f, 3.f, 1.f));
  attack = new FloatParameter(10.f, "attack", 1.f, NormalizedRange(0.f, 100.f, 1.f));
  limiterOn = new BoolParameter("limiterOn", false);
  envelope = new Sample();
  addParameter(ratio);
  addParameter(release);
  addParameter(threshold);
  addParameter(attack);
  addParameter(limiterOn);
}

#pragma warning( push )
#pragma warning( disable : 4100)
void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}
#pragma warning( pop ) 

void CompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void CompressorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  this->initializing(buffer);
  if (!this->getBypass())
  {
    processBlockwise<FilterConstants::blockSize>(
      buffer, internalBuffer, [this](size_t samples, size_t offset) 
      {
        processCompressor(internalBuffer, samples,
          getRelease(), getAttack(), getRatio(), getThreshold(),
          getLimiter(), delayLine, *envelope);
      }
    );
  }
  this->finalizing(buffer);
  this->meteringBuffer(buffer);
}

AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
	return new CompressorAudioProcessorEditor(*this);
}

void CompressorAudioProcessor::setRelease(float value) {
  release->beginChangeGesture();
  release->setUnnormalizedValue(value);
  release->endChangeGesture();
}

void CompressorAudioProcessor::setRatio(float newRatio) {
  ratio->beginChangeGesture();
  ratio->setUnnormalizedValue(newRatio);
  ratio->endChangeGesture();
}

void CompressorAudioProcessor::setAttack(float newAttack) {
  attack->beginChangeGesture();
  attack->setUnnormalizedValue(newAttack);
  attack->endChangeGesture();
}

void CompressorAudioProcessor::setThreshold(float newThreshold) {
  threshold->beginChangeGesture();
  threshold->setUnnormalizedValue(newThreshold);
  threshold->endChangeGesture();
}

void CompressorAudioProcessor::setLimiter(bool value)
{
  limiterOn->setBoolValue(value);
}

void CompressorAudioProcessor::setState(const var & state)
{
  ratio->setValue(state.getProperty("ratio", ratio->getValue()));
  attack->setValue(state.getProperty("attack", attack->getValue()));
  release->setValue(state.getProperty("release", release->getValue()));
  threshold->setValue(state.getProperty("threshold", threshold->getValue()));
  limiterOn->setBoolValue(state.getProperty("limiter", limiterOn->getBoolValue()));
}

var CompressorAudioProcessor::getState()
{
  DynamicObject* properties = new DynamicObject;
  properties->setProperty("ratio", ratio->getValue());
  properties->setProperty("attack", attack->getValue());
  properties->setProperty("release", release->getValue());
  properties->setProperty("threshold", threshold->getValue());
  properties->setProperty("limiter", limiterOn->getBoolValue());
  return var(properties);
}

float CompressorAudioProcessor::getRelease() {
  return release->getUnnormalizedValue();
}

float CompressorAudioProcessor::getRatio() {
  return ratio->getUnnormalizedValue();
}

float CompressorAudioProcessor::getAttack() {
  return attack->getUnnormalizedValue();
}

float CompressorAudioProcessor::getThreshold() {
  return threshold->getUnnormalizedValue();
}

bool CompressorAudioProcessor::getLimiter()
{
  return limiterOn->getBoolValue();
}

// Channel number is expected to be == 2
// All parameter values between 0 and 1
void processCompressor(Sample* data, size_t numberOfSamples, 
  float release, float attack, float ratio, float threshold,
  bool limiterOn,
  CircularBuffer<Sample>& delayLine, Sample& envelope)
{
  attack = attack / 1000.f;
  size_t attackTimeInSamples = static_cast<size_t>(attack * 44100);
  delayLine.setSize(attackTimeInSamples);
  release = release / 1000.f;
  float attackGain = exp(-1 / (attack * 44100));
  float releaseGain = exp(-1 / (release * 44100));
  float slope = 1 - (1 / ratio);
  for (size_t i = 0; i < numberOfSamples; i++) 
  {
    Sample input = abs(data[i]);
    if (envelope.areBothSmaller(input))
    {
      envelope = input + Sample(static_cast<double>(attackGain)) * (envelope - input);
    }
    else 
    {
      envelope = input + Sample(static_cast<double>(releaseGain)) * (envelope - input);
    }
    double envelopeValue = max(envelope);
    float envelopeDb = aux::linearToDecibel(static_cast<float>(abs(envelopeValue)));
    if (envelopeDb < threshold)
    {
      envelopeDb = 0.f;
    }
    else
    {
      if (limiterOn)
      {
        envelopeDb = (threshold - envelopeDb);
      }
      else envelopeDb = slope * (threshold - envelopeDb);
    }
    data[i] = delayLine.pushpop(data[i]);
    data[i] *= Sample(static_cast<double>(aux::decibelToLinear(envelopeDb)));
  }
}
#endif