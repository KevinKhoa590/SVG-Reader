// SVGLinearGradient.h
#pragma once
#include "SVGGradient.h"
#include <string>

class SVGLinearGradient : public SVGGradient {
public:
    SVGLinearGradient();
    // optional coordinates (x1,y1,x2,y2). If empty, treat as default [0,0] -> [1,0] in objectBoundingBox
    // stored as strings because they may be percentages; parser can convert as needed
    std::string x1, y1, x2, y2;

    // sampleAt overridden if you want special behavior — otherwise base sampleAt uses stops
    virtual SimpleColor sampleAt(double t) const override;
};
