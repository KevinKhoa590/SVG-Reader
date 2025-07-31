#include "stdafx.h"
#include "rapidxml.hpp"
#include "helper.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <regex>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
using namespace rapidxml;

Gdiplus::Color parseColor(xml_attribute<>* attr) {
    if (!attr) return Gdiplus::Color(0, 0, 0);
    std::string value = attr->value();
    int r = 0, g = 0, b = 0;
    if (sscanf_s(value.c_str(), "rgb(%d,%d,%d)", &r, &g, &b) == 3) {
        return Gdiplus::Color(r, g, b);
    }
    return Gdiplus::Color(0, 0, 0);
}

float parseOpacity(xml_attribute<>* attr) {
    if (!attr) return 1.0f;
    return std::stof(attr->value());
}

std::vector<Gdiplus::Point> parsePoints(const std::string& pointStr) {
    std::vector<Gdiplus::Point> points;
    std::regex rgx(R"((\d+),(\d+))");
    auto begin = std::sregex_iterator(pointStr.begin(), pointStr.end(), rgx);
    auto end = std::sregex_iterator();
    for (auto i = begin; i != end; ++i) {
        int x = std::stoi((*i)[1]);
        int y = std::stoi((*i)[2]);
        points.emplace_back(x, y);
    }
    return points;
}
