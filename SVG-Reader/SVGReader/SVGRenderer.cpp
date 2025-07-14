#include "stdafx.h"
#include "SVGRenderer.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

void SVGRenderer::render(const SVGParseResult& result, Gdiplus::Graphics& graphics) {
    for (const auto& element : result.elements) {
        if (element) element->draw(graphics);
    }
}
