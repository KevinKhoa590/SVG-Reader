#include "stdafx.h"
#include "SVGEllipse.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

SVGEllipse::SVGEllipse(int cx_, int cy_, int rx_, int ry_, const Color* fill, const Color* stroke, int strokeW)
    : cx(cx_), cy(cy_), rx(rx_), ry(ry_), fillColor(fill), strokeColor(stroke), strokeWidth(strokeW) {}

void SVGEllipse::draw(Graphics& g) {
    SolidBrush brush(*fillColor);
    g.FillEllipse(&brush, cx - rx, cy - ry, 2 * rx, 2 * ry);

    Pen pen(*strokeColor, strokeWidth);
    g.DrawEllipse(&pen, cx - rx, cy - ry, 2 * rx, 2 * ry);
}
