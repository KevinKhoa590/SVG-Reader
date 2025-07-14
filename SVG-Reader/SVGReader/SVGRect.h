#pragma once
#include "SVGElement.h"

namespace Gdiplus {
    class Graphics;
    class Color;
}

class SVGRect : public SVGElement {
    int x, y, width, height;
    const Gdiplus::Color* fillColor;
    const Gdiplus::Color* strokeColor;
    int strokeWidth;

public:
    SVGRect(int x, int y, int width, int height, const Gdiplus::Color* fill, const Gdiplus::Color* stroke, int strokeWidth);
    void draw(Gdiplus::Graphics& g) override;
};
