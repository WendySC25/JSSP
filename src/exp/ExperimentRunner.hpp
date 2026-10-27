#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "jssp/InstanceJSSP.hpp"
#include "RunConfig.hpp"
#include "RunResult.hpp"

class ExperimentRunner {
    private:
        std::shared_ptr<const InstanceJSSP> instance;
    
    public:
        ExperimentRunner(std::shared_ptr<const InstanceJSSP> inst);
    
        RunResult runOne(const RunConfig& config) const;
        std::vector<RunResult> run(const std::vector<RunConfig>& configs, int threads = 0) const;
    
        static std::vector<RunConfig> assignSeeds(const RunConfig& base, int runs, int baseSeed = 1);
};