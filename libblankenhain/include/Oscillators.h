// Taken from
// http://www.martin-finke.de/blog/articles/audio-plugins-018-polyblep-oscillator/
//			Oscillator.h
//			SpaceBass
//			Created by Martin on 08.04.14.
//
//
// Heavily modified

#pragma once
#include <math.h>
#include "Constants.h"
#include "Sample.h"
#include <stdint.h>

/**
 * Interface class for sound generators, processVoice Function will 
 * access sound generating modules through these two calls only.
 * Everything that generates a sound, wether periodic or not,
 * should derive from this.
 */
class I_SoundGenerator
{
public:
	virtual void setFrequency(double frequency) = 0;
	virtual float getSample(unsigned int time) = 0;
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

	virtual void setFrequency(double frequency) final;

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

	NaiveOscillator();

    enum NaiveOscillatorMode
	{
        OSCILLATOR_MODE_SINE = 0,
        OSCILLATOR_MODE_SAW,
        OSCILLATOR_MODE_SQUARE,
        OSCILLATOR_MODE_TRIANGLE,
        kNumOscillatorModes
    };

    void setMode(NaiveOscillatorMode mode);

	virtual float getSample(unsigned int time) override;

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
		return value * 4.f / constants::pi;
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

			value += osc.getSample(time) * (pow(-1.f, (k - 1u) / 2u) / (static_cast<float>(k)* static_cast<float>(k)));
			k += 2;
		}
		return value * 8.f / (constants::pi * constants::pi);
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
		return 0.5f - value * ( 1.f / (constants::pi));
	}
};

/**
 * Writes a Base Oscillator period to wavetable.
 * This provides very good performance with almost no aliasing
 * Provides good performance with no audible artifacts when base frequency (provided druing construction)
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
		const float positionInWavetable = mPhase.getValue() * static_cast<float>(size) / (2.f * constants::pi);
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
private:
	float poly_blep(float& t) const;
	float lastOutput;
};
