#pragma once
#include "warnings.h"

template <typename T>
class CircularBuffer
{
protected:
	T* buffer;
	size_t numberOfSamples;
	size_t maxNumberOfSamples;
	size_t oldNumberOfSamples;
	size_t currentPosition;
public:
	CircularBuffer(size_t numberOfSamples);
	~CircularBuffer(void);
	virtual void _vectorcall push(T in);
	virtual T _vectorcall pushpop(T in);
	size_t getSize() const;
	virtual void setSize(size_t size_);
	T _vectorcall get(int iterator = -1);
	void reset();
	size_t getCurrentIteratorInDelayline() const;
};

template <typename T>
class LinearInterpolatedCircularBuffer
	: public CircularBuffer<T>
{
public:
	LinearInterpolatedCircularBuffer(size_t numberOfSamples) : CircularBuffer(numberOfSamples) {};
	void setSize(size_t size_) override final;
private:
	T _vectorcall interpolate(T valueBegin, T valueEnd, float ratio);
};