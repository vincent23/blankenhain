#pragma once

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
	void push(T const& in);
	T pushpop(T const& in);
	size_t getSize();
	void setSize(size_t size_);
	T get(int iterator = -1);
};

