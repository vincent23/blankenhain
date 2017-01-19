// Taken from
// http://www.martin-finke.de/blog/articles/audio-plugins-018-polyblep-oscillator/
//			Oscillator.h
//			SpaceBass
//			Created by Martin on 08.04.14.
//
//
// Heavily modified
//
//
// Resources: http://lib.tkk.fi/Dipl/2007/urn009585.pdf

#pragma once
#include <math.h>
#include "Constants.h"
#include "Sample.h"
#include <stdint.h>
#include "AuxFunc.h"

/**
 * Interface class for sound generators, processVoice Function will 
 * access sound generating modules through these two calls only.
 * Everything that generates a sound, wether periodic or not,
 * should derive from this.
 *
 * For now we have two (mutually othogonal) ways to access a SoundGenerator,
 * wither via getSample(uint) or via getNextSample(). The prefered way to access is
 * get Sample(uint). getNextSample is only reasonable for periodic signals whose frequency
 * is manipulated. Some SoundGenerators, such as BaseDrum, Pulse etc. are conceptually
 * not reasonable to be accessed by getNextSample as their signals are not periodic.
 */
class I_SoundGenerator
{
public:
	virtual void setFrequency(float frequency, float detune = 0.f) = 0;

	// This is the prefered way to access a SoundGenerator
	virtual float getSample(unsigned int time) = 0;

	// This is only used in case of glide or time-dependent frequency modulation for example.
	virtual float getNextSample() = 0;
};

/**
 * Wrapps a value to range [0; 2pi].
 * Needed for Oscillators
 */
class OscillatorPhase
{
public:
	OscillatorPhase() : mPhase(0.f){}

	float getValue() const
	{
		return mPhase;
	};

	void set(float in)
	{
		if (in < 0.f)
			in *= -1;
		mPhase = in;
		mPhase = fmod(mPhase, length);
	};

	float incrementBy(float in)
	{
		if (in < 0.f)
			in *= -1;
		mPhase += in;
		mPhase = fmod(mPhase, length);
		return getValue();
	}
	static const float length;
private:
	float mPhase;
};

/**
 * A virtual basic class for periodic (i.e. oscillating) sounds.
 * A periodic sound has a frequency, so this takes care of setFrequency.
 * You still need to derive from this class and provide the getSample() function.
 * 
 * Notice the nice "render to wavetable" function
 */
class BaseOscillator : public I_SoundGenerator
{
public:
	BaseOscillator();

	/// Freq in Hertz, Detune is Value between -1.f and 1.f, where 0.f means no detune
	virtual void setFrequency(float frequency, float detune = 0.f) final;
	float getFrequency() const
	{
		return mFrequency;
	}
	void renderToWavetable(float** wavetable_, unsigned int& size, float frequency)
	{
		if (*wavetable_ != nullptr)
		{
			size = 0u;
			return;
		}
		else
		{
			this->setFrequency(frequency);
			size = getNumberOfSamplesForFullPhase();
			*wavetable_ = new float[size];
			for (unsigned int i = 0u; i < size; i++)
			{
				(*wavetable_)[i] = this->getSample(i);
			}
		}
	}

protected:
	float mFrequency;
	OscillatorPhase mPhase;
	float mPhaseIncrement;
private:
	void updateIncrement();
	unsigned int getNumberOfSamplesForFullPhase() const
	{
		return static_cast<unsigned int>(static_cast<float>(constants::sampleRate) / mFrequency) + 1u;
	}
};


/**
 * A naive, therefore aliased Oscillator for sine, square, triangle and saw.
 * PolyBlep oscillator builds on this
 */
class NaiveOscillator : public BaseOscillator
{
public:
	enum NaiveOscillatorMode
	{
		OSCILLATOR_MODE_SINE = 0,
		OSCILLATOR_MODE_SAW,
		OSCILLATOR_MODE_SQUARE,
		OSCILLATOR_MODE_TRIANGLE,
		kNumOscillatorModes
	};

	NaiveOscillator();

	NaiveOscillator(NaiveOscillatorMode mode, float frequency)
	{
		this->setMode(mode);
		this->setFrequency(frequency);
	}

    void setMode(NaiveOscillatorMode mode);

	virtual float getSample(unsigned int time) override;

	virtual float getNextSample() override;

protected:
    float naiveWaveformForMode(NaiveOscillatorMode mode);
	NaiveOscillatorMode mOscillatorMode;
};

// via https://www.music.mcgill.ca/~gary/307/week5/bandlimited.html
/**
 * An additive square wave oscilaltor
 * as it is additive, there is no aliasing.
 * very slow, you should only use this to render to a wavetable
 */
class AdditiveSquareWaveOscillator : public BaseOscillator
{
public:
	virtual float getSample(unsigned int time) final
	{
		mPhase.set(static_cast<float>(time) * mPhaseIncrement);
		float value = 0.f;
		unsigned int k = 1u;
		float currentHarmonic = 0.f;
		NaiveOscillator osc;
		osc.setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		while (currentHarmonic < constants::sampleRate / 2.f)
		{
			currentHarmonic = static_cast<float>(k) * mFrequency;
			osc.setFrequency(currentHarmonic);
			
			value += osc.getSample(time) * (1.f / static_cast<float>(k));
			k += 2;
		}
		return value * 4.f / static_cast<float>(constants::pi);
	}

	virtual float getNextSample() final
	{
		mPhase.incrementBy(mPhaseIncrement);
		float value = 0.f;
		unsigned int k = 1u;
		float currentHarmonic = 0.f;
		NaiveOscillator osc;
		osc.setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		while (currentHarmonic < constants::sampleRate / 2.f)
		{
			currentHarmonic = static_cast<float>(k) * mFrequency;
			osc.setFrequency(currentHarmonic);

			value += osc.getNextSample() * (1.f / static_cast<float>(k));
			k += 2;
		}
		return value * 4.f / static_cast<float>(constants::pi);
	}
};

// via https://www.music.mcgill.ca/~gary/307/week5/bandlimited.html
/**
* An additive triangle wave oscilaltor
* as it is additive, there is no aliasing.
* very slow, you should only use this to render to a wavetable
*/
class AdditiveTriangleWaveOscillator : public BaseOscillator
{
public:
	virtual float getSample(unsigned int time) final
	{
		mPhase.set(static_cast<float>(time) * mPhaseIncrement);
		float value = 0.f;
		unsigned int k = 1u;
		float currentHarmonic = 0.f;
		NaiveOscillator osc;
		osc.setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		while (currentHarmonic < constants::sampleRate / 2.f)
		{
			currentHarmonic = static_cast<float>(k) * mFrequency;
			osc.setFrequency(currentHarmonic);

			value += osc.getSample(time) * (powf(-1.f, (k - 1u) / 2u) / (static_cast<float>(k)* static_cast<float>(k)));
			k += 2;
		}
		return value * 8.f / static_cast<float>(constants::pi * constants::pi);
	}

	virtual float getNextSample() final
	{
		mPhase.incrementBy(mPhaseIncrement);
		float value = 0.f;
		unsigned int k = 1u;
		float currentHarmonic = 0.f;
		NaiveOscillator osc;
		osc.setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		while (currentHarmonic < constants::sampleRate / 2.f)
		{
			currentHarmonic = static_cast<float>(k) * mFrequency;
			osc.setFrequency(currentHarmonic);

			value += osc.getNextSample() * (powf(-1.f, (k - 1u) / 2u) / (static_cast<float>(k)* static_cast<float>(k)));
			k += 2;
		}
		return value * 8.f / static_cast<float>(constants::pi * constants::pi);
	}
};

// via https://www.music.mcgill.ca/~gary/307/week5/bandlimited.html
/**
* An additive triangle wave oscilaltor
* as it is additive, there is no aliasing.
* very slow, you should only use this to render to a wavetable
*/
class AdditiveSawtoothWaveOscillator : public BaseOscillator
{
public:
	virtual float getSample(unsigned int time) final
	{
		mPhase.set(static_cast<float>(time) * mPhaseIncrement);
		float value = 0.f;
		unsigned int k = 1u;
		float currentHarmonic = 0.f;
		NaiveOscillator osc;
		osc.setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		while (currentHarmonic < constants::sampleRate / 2.f)
		{
			currentHarmonic = static_cast<float>(k) * mFrequency;
			osc.setFrequency(currentHarmonic);

			value += osc.getSample(time)  / (static_cast<float>(k));
			k += 1;
		}
		return 0.5f - value * ( 1.f / static_cast<float>(constants::pi));
	}

	virtual float getNextSample() final
	{
		mPhase.incrementBy(mPhaseIncrement);
		float value = 0.f;
		unsigned int k = 1u;
		float currentHarmonic = 0.f;
		NaiveOscillator osc;
		osc.setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		while (currentHarmonic < constants::sampleRate / 2.f)
		{
			currentHarmonic = static_cast<float>(k) * mFrequency;
			osc.setFrequency(currentHarmonic);

			value += osc.getNextSample() / (static_cast<float>(k));
			k += 1;
		}
		return 0.5f - value * (1.f / static_cast<float>(constants::pi));
	}
};

/**
 * Writes a Base Oscillator period to wavetable.
 * This provides very good performance with almost no aliasing
 * Provides good performance with no audible artifacts when base frequency (provided during construction)
 * is small enough.
 */
class WaveTableOscillator : public BaseOscillator
{
private:
	float* wavetable;
	unsigned int size;
public:
	WaveTableOscillator() = delete;
	WaveTableOscillator(BaseOscillator& base, float waveTableBaseFrequency = 8.5f)
		: size(0u), wavetable(nullptr)
	{
		if (wavetable == nullptr)
		{
			base.renderToWavetable(&wavetable, size, waveTableBaseFrequency);
		}
	}

	~WaveTableOscillator()
	{
		if (wavetable != nullptr)
		{
			delete[] wavetable;
			wavetable = nullptr;
		}
	}

	virtual float getSample(unsigned int time) final
	{
		mPhase.set(static_cast<float>(time) * mPhaseIncrement);
		const float positionInWavetable = mPhase.getValue() * static_cast<float>(size) / static_cast<float>(2.f * constants::pi);
		return wavetable[static_cast<unsigned int>(positionInWavetable)];
	}

	virtual float getNextSample() final
	{
		mPhase.incrementBy(mPhaseIncrement);
		const float positionInWavetable = mPhase.getValue() * static_cast<float>(size) / static_cast<float>(2.f * constants::pi);
		return wavetable[static_cast<unsigned int>(positionInWavetable)];
	}
};

/**
 * Creates white noise, is not an oscillator to be honest
 * since random values are calculated upon every call to
 * getSample. So it is not 100% periodical.
 *
 * via http://stackoverflow.com/questions/13213395/adjusting-xorshift-generator-to-return-a-number-within-a-maximum
 * and https://en.wikipedia.org/wiki/Xorshift
 * and http://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
 */
class NoiseOscillator : public BaseOscillator
{
private:
	uint32_t state;
	uint32_t xorshift32(void)
	{
		uint32_t x = state;
		x ^= x << 13;
		x ^= x >> 17;
		x ^= x << 5;
		state = x;
		return x;
	}
public:
	NoiseOscillator()
	{
		state = 12334u;
		for (unsigned int i = 0u; i < 5078; i++)
			this->xorshift32();
		state = 634u;
		for (unsigned int i = 0u; i < 578; i++)
			this->xorshift32();
	}

	virtual float getSample(unsigned int time) final
	{
		this->xorshift32();
		while (state > 4000000001 || state == 0u)
			this->xorshift32();
		return (static_cast<float>(state) - 1.f) / 4000000000.f;
	}

	virtual float getNextSample() final
	{
		return this->getSample(0u);
	}
};


/**
* adapted from http://www.martin-finke.de/blog/articles/audio-plugins-018-polyblep-oscillator/
* PolyBLEP by Tale
* (slightly modified)
* 2 sample polynomal for BLEP, from
* http://www.kvraudio.com/forum/viewtopic.php?t=375517
*
* polyBLEP Oscillator using two samples to smoothen and first order integrated polynomial
* better polynamials (for the future) can be found in:  http://lib.tkk.fi/Dipl/2007/urn009585.pdf
* as well as
* http://www.kvraudio.com/forum/viewtopic.php?f=33&t=398553
*/
class PolyBLEPOscillator : public NaiveOscillator {
public:
	PolyBLEPOscillator() : NaiveOscillator(), lastOutput(0.0)
	{
	};

	virtual float getSample(unsigned int time) final;
	virtual float getNextSample() final
	{
		float value = 0.0;

		mPhase.incrementBy(mPhaseIncrement);

		float t = mPhase.getValue() / (2.f * constants::pi);

		if (mOscillatorMode == OSCILLATOR_MODE_SINE) {
			value = naiveWaveformForMode(OSCILLATOR_MODE_SINE);
		}
		else if (mOscillatorMode == OSCILLATOR_MODE_SAW) {
			value = naiveWaveformForMode(OSCILLATOR_MODE_SAW);
			value -= poly_blep(t);
		}
		else {
			value = naiveWaveformForMode(OSCILLATOR_MODE_SQUARE);
			value += poly_blep(t);
			float temp = fmod(t + 0.5f, 1.f);
			value -= poly_blep(temp);
			if (mOscillatorMode == OSCILLATOR_MODE_TRIANGLE) 
			{
				// Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
				value = mPhaseIncrement * value + (1. - mPhaseIncrement) * lastOutput;
				lastOutput = value;
			}
		}
		return value;
	}
private:
	float poly_blep(float t) const;
	float lastOutput;
};

/**
 * Creates a Pulse of defined length:
 *
 * _______*******______
 *
* (Sounds like shit)
 *
 * Useful for drum synthesis and control logic.
 */
class PulseSound : public I_SoundGenerator
{
public:
	PulseSound() : pulseLengthInSamples(1u) {}
	PulseSound(unsigned int pulseLengthInSamples_) : pulseLengthInSamples(pulseLengthInSamples_) {}
	unsigned int pulseLengthInSamples;
	virtual void setFrequency(float frequency, float detune) final {};
	virtual float getSample(unsigned int time) final
	{
		if (time <= pulseLengthInSamples)
			return 1.f;
		else return 0.f;
	}

	virtual float getNextSample() final
	{
		return 0.f;
	}
};

