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
#include "Constants.h"
#include "Sample.h"
#include "AuxFunc.h"
#include "BhMath.h"

/**
* Wrapps a value to range [0; 2pi].
* Needed for Oscillators.
* Is able to handle negative values by
*
*/
class OscillatorPhase
{
public:
	OscillatorPhase() : mPhase(0.f) {}

	OscillatorPhase(float in) : mPhase(0.f)
	{
		this->set(in);
	}


	const float& getValue() const
	{
		return mPhase;
	};

	void set(float in)
	{
		if (in < 0.f)
		{
			in = BhMath::fmod(in, length);
			in += length;
			mPhase = in;
		}
		else
		{
			mPhase = in;
			mPhase = BhMath::fmod(mPhase, length);
		}
	};

	const float& incrementBy(float in)
	{
		if (in < 0.f)
			in *= -1;
		mPhase += in;
		mPhase = BhMath::fmod(mPhase, length);
		return getValue();
	}
	const float length = 2.f * static_cast<float>(constants::pi);
private:
	float mPhase;
};

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
	virtual void setFrequency(float frequency) = 0;

	// This is the prefered way to access a SoundGenerator
	virtual float getSample(unsigned int time, OscillatorPhase phase = OscillatorPhase()) = 0;

	// This is only used in case of glide or time-dependent frequency modulation for example.
	virtual float getNextSample(OscillatorPhase phase = OscillatorPhase()) = 0;
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
	virtual void setFrequency(float frequency) final;
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

	virtual float getSample(unsigned int time, OscillatorPhase phase = OscillatorPhase()) override;

	virtual float getNextSample(OscillatorPhase phase = OscillatorPhase()) override;

protected:
    float naiveWaveformForMode(NaiveOscillatorMode mode, OscillatorPhase phase = OscillatorPhase());
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
	virtual float getSample(unsigned int time, OscillatorPhase phase = OscillatorPhase()) final
	{
		float value = 0.f;
		unsigned int k = 1u;
		float currentHarmonic = 0.f;
		NaiveOscillator osc;
		osc.setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		while (currentHarmonic < constants::sampleRate / 2.f)
		{
			currentHarmonic = static_cast<float>(k) * mFrequency;
			osc.setFrequency(currentHarmonic);
			
			value += osc.getSample(time, phase) * (1.f / static_cast<float>(k));
			k += 2;
		}
		return value * 4.f / static_cast<float>(constants::pi);
	}

	virtual float getNextSample(OscillatorPhase phase = OscillatorPhase()) final
	{
		float value = 0.f;
		unsigned int k = 1u;
		float currentHarmonic = 0.f;
		NaiveOscillator osc;
		osc.setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		while (currentHarmonic < constants::sampleRate / 2.f)
		{
			currentHarmonic = static_cast<float>(k) * mFrequency;
			osc.setFrequency(currentHarmonic);

			value += osc.getNextSample(phase) * (1.f / static_cast<float>(k));
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
	virtual float getSample(unsigned int time, OscillatorPhase phase = OscillatorPhase()) final
	{
		float value = 0.f;
		unsigned int k = 1u;
		float currentHarmonic = 0.f;
		NaiveOscillator osc;
		osc.setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		while (currentHarmonic < constants::sampleRate / 2.f)
		{
			currentHarmonic = static_cast<float>(k) * mFrequency;
			osc.setFrequency(currentHarmonic);

			value += osc.getSample(time, phase) * (BhMath::pow(-1.f, static_cast<float>((k - 1u) / 2u)) / (static_cast<float>(k)* static_cast<float>(k)));
			k += 2;
		}
		return value * 8.f / static_cast<float>(constants::pi * constants::pi);
	}

	virtual float getNextSample(OscillatorPhase phase = OscillatorPhase()) final
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

			value += osc.getNextSample(phase) * (BhMath::pow(-1.f, static_cast<float>((k - 1u) / 2u)) 
				/ (static_cast<float>(k) * static_cast<float>(k)));
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
	virtual float getSample(unsigned int time, OscillatorPhase phase = OscillatorPhase()) final
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

			value += osc.getSample(time, phase)  / (static_cast<float>(k));
			k += 1;
		}
		return 0.5f - value * ( 1.f / static_cast<float>(constants::pi));
	}

	virtual float getNextSample(OscillatorPhase phase = OscillatorPhase()) final
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

			value += osc.getNextSample(phase) / (static_cast<float>(k));
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
	WaveTableOscillator(BaseOscillator& base, float waveTableBaseFrequency = 30.f)
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

	virtual float getSample(unsigned int time, OscillatorPhase phase = OscillatorPhase()) final
	{
		mPhase.set(static_cast<float>(time) * mPhaseIncrement);
		const float positionInWavetable = (mPhase.getValue() + phase.getValue()) * static_cast<float>(size) / static_cast<float>(2.f * constants::pi);
		return wavetable[static_cast<unsigned int>(positionInWavetable)];
	}

	virtual float getNextSample(OscillatorPhase phase = OscillatorPhase()) final
	{
		mPhase.incrementBy(mPhaseIncrement);
		const float positionInWavetable = ( mPhase.getValue() + phase.getValue()) * static_cast<float>(size) / static_cast<float>(2.f * constants::pi);
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

	virtual float getSample(unsigned int time, OscillatorPhase phase = OscillatorPhase()) final
	{
		this->xorshift32();
		while (state > 4000000001 || state == 0u)
			this->xorshift32();
		return (((static_cast<float>(state) - 1.f) / 4000000000.f) - 0.5f) * 2.f;
	}

	virtual float getNextSample(OscillatorPhase phase = OscillatorPhase()) final
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

	virtual float getSample(unsigned int time, OscillatorPhase phase = OscillatorPhase()) final;
	virtual float getNextSample(OscillatorPhase phase = OscillatorPhase()) final;
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
	virtual void setFrequency(float frequency) final {};
	virtual float getSample(unsigned int time, OscillatorPhase phase = OscillatorPhase()) final
	{
		if (time <= pulseLengthInSamples)
			return 1.f;
		else return 0.f;
	}

	virtual float getNextSample(OscillatorPhase phase = OscillatorPhase()) final
	{
		return 0.f;
	}
};

