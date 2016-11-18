/*
  ==============================================================================

    UpDownSampling.h
    Created: 4 Apr 2016 9:13:32pm
    Author:  Dustin Kaiser

  ==============================================================================
*/

#include "Sample.h"
#include <cmath>

#pragma once

// Upsampling using Spine Interpolation, watch the syntax (differs from upsampleZeros)
Sample* upsampleSpline(Sample* buffer, size_t originalSamples, size_t interpolatedSamples);

// Upsampling by inserting zeros, watch the syntax (differs from upsampleSpline)
Sample* upsampleZeros(Sample* buffer, size_t originalSamples, size_t multiplier);

// Downsampling currently only to 2**n fractions of original size
Sample* simpleDownsample(Sample* buffer, size_t originalSamples, size_t divisor);

namespace spline
{
  // all via http://www.geos.ed.ac.uk/~yliu23/docs/lect_spline.pdf

  void solveTridiagonal(double* a, double* b, double* c, double* d, int n);

  double B_0fkt(double x, double x0, double h = 1.);

  double B_fkt(double k, double x, double x0, double h = 1.);
}

