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
	virtual void push(T const& in);
	virtual T pushpop(T const& in);
	size_t getSize() const;
	virtual void setSize(size_t size_);
	virtual T get(int iterator = -1);
};

template <typename T>
class LinearInterpolatedCircularBuffer
	: public CircularBuffer<T>
{
public:
	LinearInterpolatedCircularBuffer(size_t numberOfSamples) : CircularBuffer(numberOfSamples) {};
	void setSize(size_t size_) override;
private:
	T interpolate(T& valueBegin, T& valueEnd, float ratio);
};