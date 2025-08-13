// SVGLinearGradient.cpp
#include "stdafx.h"
#include "SVGLinearGradient.h"
#include <algorithm>

SVGLinearGradient::SVGLinearGradient() : SVGGradient() {
}

SimpleColor SVGLinearGradient::sampleAt(double t) const {
    // For linear gradient, the sample at t is same as base implementation (offset-based)
    return SVGGradient::sampleAt(t);
}
