#pragma once

#include <memory>
#include "Solution.hpp"
#include "Move.hpp"

class Solution;

struct Candidate {
    std::shared_ptr<const Solution> parent;
    Move move;
    double cost;
};
