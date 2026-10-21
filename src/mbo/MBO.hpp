
#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <stdexcept>

#include "MBOConfig.hpp"
#include "MBOResult.hpp"
#include "Bird.hpp"

class MBO {
    private:

        std::shared_ptr<const Solution> sol;
        
        MBOconfig params;

        std::mt19937 rng;
        std::vector<Candidate> candidates;
        std::vector<std::unique_ptr<Bird> > flock;

        std::shared_ptr<const Solution> best;
        
        int evals = 0;

        void initFlock();
        void step(int i);
        int frontOf(int i);

    public:
        MBO(std::shared_ptr<const Solution> sol, MBOconfig params);
        MBOResult run(std::uint32_t seed);
};
