// SVGRadialGradient.h
#pragma once
#include "SVGGradient.h"
#include <string>

class SVGRadialGradient : public SVGGradient {
public:
    SVGRadialGradient();
    // center / focal coords as strings (may be userSpaceOnUse or percentages)
    std::string cx, cy, r, fx, fy;
    // if renderer cannot do radial, callers should use averageColor()
};
