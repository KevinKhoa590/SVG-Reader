#include "stdafx.h"
#include "SVGGroup.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

SVGGroup::SVGGroup() {}

SVGGroup::~SVGGroup() {
    // No need to manually delete; smart pointers handle cleanup.
    children.clear();
}

void SVGGroup::addElement(std::unique_ptr<SVGElement> child) {
    children.push_back(std::move(child));
}

void SVGGroup::draw(Gdiplus::Graphics& g) {
    for (auto& child : children) {
        if (child)
            child->elementRender(g);
    }
}
