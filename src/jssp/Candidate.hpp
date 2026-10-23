#pragma once

#include <memory>
#include "Solution.hpp"
#include "Move.hpp"

class Solution;

struct Candidate {
    std::shared_ptr<const Solution> parent;
    Move move;
    double cost;

    bool operator<(const Candidate& other) const {
        return cost < other.cost;
    }
};
