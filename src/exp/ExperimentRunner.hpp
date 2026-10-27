#pragma once

#include <vector>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <algorithm>
#include <random>
#include <atomic>

#include "jssp/InstanceJSSP.hpp"
#include "jssp/SolutionJSSP.hpp"
#include "mbo/MBO.hpp"
#include "RunConfig.hpp"
#include "RunResult.hpp"

class ExperimentRunner {
    private:
        std::shared_ptr<const InstanceJSSP> instance;
        RunResult runOne(const RunConfig& config) const;

        /**
         * @brief Generates a string representation of the current time.
         * @return The formatted timestamp.
         */
        std::string nowTimestamp() const;
    
    public:
        ExperimentRunner(std::shared_ptr<const InstanceJSSP> inst);
    
        std::vector<RunResult> run(const std::vector<RunConfig>& configs, int threads = 0) const;

        static std::vector<RunConfig> assignSeeds(const RunConfig& base, int runs, int baseSeed = 1);
};