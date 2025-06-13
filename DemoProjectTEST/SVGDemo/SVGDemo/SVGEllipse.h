#pragma once
#include "SVGElement.h"

namespace Gdiplus {
    class Graphics;
    class Color;
}

class SVGEllipse : public SVGElement {
    int cx, cy, rx, ry;
    const Gdiplus::Color* fillColor;
    const Gdiplus::Color* strokeColor;
    int strokeWidth;

public:
    SVGEllipse(int cx, int cy, int rx, int ry, const Gdiplus::Color* fill, const Gdiplus::Color* stroke, int strokeWidth);
    void draw(Gdiplus::Graphics& g) override;
};
