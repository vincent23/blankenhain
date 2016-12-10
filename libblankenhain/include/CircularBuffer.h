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
	size_t getSize();
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

template <typename T>
class OnePoleFilter : public AlignedType
{
protected:
  T param_b0;
  T param_a1;
  T gain;
  T inputValue, outputValue0;
public:
  OnePoleFilter(T const& polePosition);
  ~OnePoleFilter(void);
  T tick(T const& in);
  void setParams(T const& b0, T const&a1);
  void setGain(T const& gain);
  T getGain() const;
  void getParams(T & b0, T& a1) const;
  void setPole(T const& polePosition);
};