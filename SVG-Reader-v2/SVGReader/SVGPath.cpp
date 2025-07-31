#include "stdafx.h"
#include "SVGPath.h"
#include "helper.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <sstream>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

SVGPath::SVGPath() : strokeColor(nullptr), fillColor(nullptr) {}

SVGPath::~SVGPath() {
    delete strokeColor;
    delete fillColor;
}

void SVGPath::setAttribute(xml_attribute<>* attr) {
    std::wstring name = std::wstring(attr->name(), attr->name() + strlen(attr->name()));
    std::wstring value = std::wstring(attr->value(), attr->value() + strlen(attr->value()));

    if (name == L"d") {
        d = value;
    }
    else if (name == L"stroke") {
        delete strokeColor;
        strokeColor = new Color(parseColor(attr));
        hasStroke = true;
    }
    else if (name == L"fill") {
        delete fillColor;
        fillColor = new Color(parseColor(attr));
        hasFill = true;
    }
    else if (name == L"stroke-width") {
        strokeWidth = std::stof(value);
    }
    else if (name == L"stroke-opacity") {
        strokeOpacity = parseOpacity(attr);
    }
    else if (name == L"fill-opacity") {
        fillOpacity = parseOpacity(attr);
    }
}

void SVGPath::draw(Graphics& g) {
    GraphicsPath path;
    std::wstringstream ss(d);
    wchar_t cmd;
    PointF curr{ 0, 0 }, start{ 0, 0 };

    while (ss >> cmd) {
        if (cmd == L'M') {
            float x, y;
            ss >> x;
            if (ss.peek() == L',') ss.ignore();
            ss >> y;
            curr = { x, y };
            start = curr;
        }
        else if (cmd == L'L') {
            float x, y;
            ss >> x;
            if (ss.peek() == L',') ss.ignore();
            ss >> y;
            PointF dest = { x, y };
            path.AddLine(curr, dest);
            curr = dest;
        }
        else if (cmd == L'H') {
            float x;
            ss >> x;
            PointF dest = { x, curr.Y };
            path.AddLine(curr, dest);
            curr = dest;
        }
        else if (cmd == L'V') {
            float y;
            ss >> y;
            PointF dest = { curr.X, y };
            path.AddLine(curr, dest);
            curr = dest;
        }
        else if (cmd == L'C') {
            float x1, y1, x2, y2, x, y;
            ss >> x1; if (ss.peek() == L',') ss.ignore();
            ss >> y1; if (ss.peek() == L',') ss.ignore();
            ss >> x2; if (ss.peek() == L',') ss.ignore();
            ss >> y2; if (ss.peek() == L',') ss.ignore();
            ss >> x;  if (ss.peek() == L',') ss.ignore();
            ss >> y;
            path.AddBezier(curr, { x1, y1 }, { x2, y2 }, { x, y });
            curr = { x, y };
        }
        else if (cmd == L'Z' || cmd == L'z') {
            path.CloseFigure();
            curr = start;
        }
    }

    if (hasFill && fillOpacity > 0 && fillColor) {
        SolidBrush brush(Color(
            static_cast<BYTE>(fillOpacity * 255),
            fillColor->GetR(),
            fillColor->GetG(),
            fillColor->GetB()
        ));
        g.FillPath(&brush, &path);
    }

    if (hasStroke && strokeOpacity > 0 && strokeWidth > 0 && strokeColor) {
        Pen pen(Color(
            static_cast<BYTE>(strokeOpacity * 255),
            strokeColor->GetR(),
            strokeColor->GetG(),
            strokeColor->GetB()
        ), strokeWidth);
        g.DrawPath(&pen, &path);
    }
}
