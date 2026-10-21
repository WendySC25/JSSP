#pragma once

#include<memory>
#include<vector>

#include "jssp/Solution.hpp"

struct Bird {
    std::shared_ptr<const Solution> solution;
    std::vector<Candidate> leftovers;
};