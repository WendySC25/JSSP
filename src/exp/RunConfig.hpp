#pragma once

#include "mbo/MBOConfig.hpp"
#include "jssp/Solution.hpp"


struct RunConfig {
    MBOconfig params;
    std::string label;
    bool randomStart = false;
    int seed = 0;
};
