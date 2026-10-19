#pragma once
#include<string>
#include<random>

#include "Candidate.hpp"

/**
 * @brief Abstract base class representing a generic solution for optimization.
 */

// Same as TSP but better xd
class Solution {
    protected:
        /** Current cost or objective function value of the solution. */
        mutable double cost;

    public:

        virtual std::shared_ptr<const Solution> createRandom(std::mt19937& rng) const = 0;
        /**
         * @brief Proposes a new neighbor solution.
         * @return Move to generate the proposed neighbor.
         */
        virtual std::pair<Move, double> proposeNeighbor(std::mt19937& rng) const = 0;

        /**
         * @brief Accepts the proposed neighbor by aplting the Move, making it the current solution.
         */
        virtual void acceptPropose(const Move& move, double newCost) const = 0;;

        /**
         * @brief Returns a string representation of the solution state.
         * @return Formatted string with solution details.
         */
        virtual std::string toString() = 0;

        /**
         * @brief Retrieves the current cost of the solution.
         * @return The current cost.
         */
        double getCost(){
            return cost;
        }
};