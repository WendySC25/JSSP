#pragma once

#include <random>
#include "jssp/InstanceJSSP.hpp"

class SolutionJSSP{
    private:
        double cost;    
        std::vector<int> order;

        void generateRandomSolution(const InstanceJSSP& inst, std::mt19937& rng);

    public:
        SolutionJSSP(const InstanceJSSP& inst, std::mt19937& rng);
        SolutionJSSP(std::vector<int> order);

};
