#pragma once

#include<memory>
#include<vector>

#include "jssp/SolutionJSSP.hpp"

struct Bird {
    std::shared_ptr<const SolutionJSSP> solution;
    std::vector<Candidate> leftovers;
};