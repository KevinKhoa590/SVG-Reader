#pragma once

#include "SVGElement.h"
#include <vector>
#include <memory>

namespace Gdiplus {
    class Graphics;
}

class SVGGroup : public SVGElement {
private:
    std::vector<std::unique_ptr<SVGElement>> children;

public:
    SVGGroup();
    ~SVGGroup();

    void addElement(std::unique_ptr<SVGElement> child);
    void draw(Gdiplus::Graphics& g) override;
};
