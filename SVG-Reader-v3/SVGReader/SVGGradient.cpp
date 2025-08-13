// SVGGradient.cpp
#include "stdafx.h"
#include "SVGGradient.h"
#include <algorithm>
#include <cmath>

static double clamp01(double v) {
    if (v < 0.0) return 0.0;
    if (v > 1.0) return 1.0;
    return v;
}

static SimpleColor lerpColor(const SimpleColor& a, const SimpleColor& b, double t) {
    auto lerp = [&](unsigned char A, unsigned char B)->unsigned char {
        double v = A + (B - A) * t;
        if (v < 0) v = 0;
        if (v > 255) v = 255;
        return static_cast<unsigned char>(std::round(v));
        };
    return SimpleColor(lerp(a.r, b.r), lerp(a.g, b.g), lerp(a.b, b.b),
        lerp(a.a, b.a));
}

SimpleColor SVGGradient::sampleAt(double t) const {
    if (stops.empty()) return SimpleColor(0, 0, 0, 255);
    t = clamp01(t);

    // If only one stop, return it
    if (stops.size() == 1) {
        SimpleColor c = stops[0].color;
        double alpha = stops[0].opacity;
        c.a = static_cast<unsigned char>(std::round(c.a * alpha));
        return c;
    }

    // Ensure stops sorted by offset
    std::vector<GradientStop> s = stops;
    std::sort(s.begin(), s.end(), [](const GradientStop& a, const GradientStop& b) {
        return a.offset < b.offset;
        });

    // Find surrounding stops
    if (t <= s.front().offset) {
        SimpleColor c = s.front().color;
        double alpha = s.front().opacity;
        c.a = static_cast<unsigned char>(std::round(c.a * alpha));
        return c;
    }
    if (t >= s.back().offset) {
        SimpleColor c = s.back().color;
        double alpha = s.back().opacity;
        c.a = static_cast<unsigned char>(std::round(c.a * alpha));
        return c;
    }
    for (size_t i = 0; i + 1 < s.size(); ++i) {
        const GradientStop& g0 = s[i];
        const GradientStop& g1 = s[i + 1];
        if (t >= g0.offset && t <= g1.offset) {
            double span = g1.offset - g0.offset;
            double local = (span <= 0.0) ? 0.0 : (t - g0.offset) / span;
            // interpolate colors and opacities
            SimpleColor c0 = g0.color;
            SimpleColor c1 = g1.color;
            // pre-multiply alpha by stop opacity
            double a0 = (g0.opacity) * (c0.a / 255.0);
            double a1 = (g1.opacity) * (c1.a / 255.0);
            // lerp channels
            SimpleColor out = lerpColor(c0, c1, local);
            // compute alpha separately and set
            double outA = a0 + (a1 - a0) * local;
            out.a = static_cast<unsigned char>(std::round(outA * 255.0));
            return out;
        }
    }
    // fallback
    return s.back().color;
}

SimpleColor SVGGradient::averageColor() const {
    if (stops.empty()) return SimpleColor(0, 0, 0, 255);
    // approximate integral by sampling or weighted average of stops
    // We'll compute area-weighted average using stop midpoints
    std::vector<GradientStop> s = stops;
    std::sort(s.begin(), s.end(), [](const GradientStop& a, const GradientStop& b) {
        return a.offset < b.offset;
        });
    // ensure first at 0 and last at 1
    if (s.front().offset > 0.0) {
        GradientStop head = s.front();
        head.offset = 0.0;
        s.insert(s.begin(), head);
    }
    if (s.back().offset < 1.0) {
        GradientStop tail = s.back();
        tail.offset = 1.0;
        s.push_back(tail);
    }
    double totalWeight = 0.0;
    double r = 0, g = 0, b = 0, a = 0;
    for (size_t i = 0; i + 1 < s.size(); ++i) {
        double left = s[i].offset;
        double right = s[i + 1].offset;
        double mid = (left + right) / 2.0;
        double weight = right - left;
        SimpleColor c = sampleAt(mid);
        double alpha = c.a / 255.0;
        r += (c.r * alpha) * weight;
        g += (c.g * alpha) * weight;
        b += (c.b * alpha) * weight;
        a += (alpha)*weight;
        totalWeight += weight;
    }
    if (totalWeight <= 0.0) totalWeight = 1.0;
    double rr = r / totalWeight;
    double gg = g / totalWeight;
    double bb = b / totalWeight;
    double aa = a / totalWeight;
    unsigned char outA = static_cast<unsigned char>(std::round(aa * 255.0));
    unsigned char outR = static_cast<unsigned char>(std::round(rr));
    unsigned char outG = static_cast<unsigned char>(std::round(gg));
    unsigned char outB = static_cast<unsigned char>(std::round(bb));
    return SimpleColor(outR, outG, outB, outA);
}
