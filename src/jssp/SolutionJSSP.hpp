#pragma once

#include <random>
#include <algorithm>
#include "InstanceJSSP.hpp"
#include "Solution.hpp"
#include "CostFunction.hpp"

class SolutionJSSP : public Solution  {
    private:

        std::shared_ptr<CostFunction> costFunction;
        bool randomStart = false;
        mutable std::vector<int> order;
        std::vector<int> posOfOp;
        std::vector<int> generateRandomOrder(const InstanceJSSP& inst, std::mt19937& rng);

    public:
        SolutionJSSP(std::shared_ptr<CostFunction> costFunc, std::mt19937& rng, bool randomStart);
        SolutionJSSP(std::shared_ptr<CostFunction> costFunc, std::vector<int> order, std::vector<int> pos, double cost);

        std::shared_ptr<const Solution> createRandom(std::mt19937& rng) const override;
        Candidate proposeNeighbor(std::mt19937& rng) const override;
        std::shared_ptr<const Solution> materializePropose(const Move& move, double newCost) const override;

        std::string toString() const override;
        std::vector<int> getOrder() const { return order; };

};

