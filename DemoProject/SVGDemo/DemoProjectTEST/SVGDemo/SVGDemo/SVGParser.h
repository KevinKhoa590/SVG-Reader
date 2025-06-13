#pragma once
#include <vector>
#include <string>
#include <memory>
#include "SVGElement.h"

class SVGParser {
public:
    static std::vector<std::unique_ptr<SVGElement>> parse(const std::string& filename);
};
