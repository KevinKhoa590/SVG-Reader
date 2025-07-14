#pragma once
#include "stdafx.h"
#include "SVGParser.h"

namespace Gdiplus {
    class Graphics;
}

class SVGRenderer {
public:
    void render(const SVGParseResult& result, Gdiplus::Graphics& graphics);
};
