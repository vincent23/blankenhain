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
	virtual void _vectorcall push(const T& in);
	virtual T _vectorcall pushpop(const T& in);
	const size_t& getSize() const;
	virtual void setSize(size_t size_);
	const T& _vectorcall get(int iterator = -1);
	void reset();
	size_t const& getCurrentIteratorInDelayline() const;
};

template <typename T>
class LinearInterpolatedCircularBuffer
	: public CircularBuffer<T>
{
public:
	LinearInterpolatedCircularBuffer(size_t numberOfSamples) : CircularBuffer(numberOfSamples) {};
	void setSize(size_t size_) override final;
private:
	T _vectorcall interpolate(const T& valueBegin, const T& valueEnd, float ratio);
};