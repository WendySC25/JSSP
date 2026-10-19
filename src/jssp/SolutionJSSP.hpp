#pragma once

#include <random>
#include "InstanceJSSP.hpp"
#include "Solution.hpp"
#include "CostFunction.hpp"

class SolutionJSSP : public Solution {
    private:

        std::shared_ptr<const InstanceJSSP> instance; 
        std::shared_ptr<CostFunction> costFunction;
        mutable std::vector<int> order;

        static std::vector<int> generateRandomOrder(const InstanceJSSP& inst, std::mt19937& rng);

    public:
        SolutionJSSP(std::shared_ptr<const InstanceJSSP> inst, std::shared_ptr<CostFunction> costFunc, std::mt19937& rng);
        SolutionJSSP(std::shared_ptr<const InstanceJSSP> inst, std::shared_ptr<CostFunction> costFunc, std::vector<int> order, double cost);

        std::shared_ptr<const Solution> createRandom(std::mt19937& rng) const override;
        std::pair<Move, double> proposeNeighbor(std::mt19937& rng) const override;
        std::shared_ptr<const Solution> materializePropose(const Move& move, double newCost) const override;

        std::string toString() override;

};

