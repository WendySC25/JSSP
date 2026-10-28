#pragma once

#include "RunConfig.hpp"

struct RunResult {
    RunConfig config;
    
    std::string error;     
    bool ok = false;    

    double cost = 0.0;
    int makespan = -1;
    int nodesCycles = -1;
    double seconds = 0.0; 

    std::string timestamp;
    std::string solutionString;
};
