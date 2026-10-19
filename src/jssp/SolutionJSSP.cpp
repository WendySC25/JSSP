#include "jssp/SolutionJSSP.hpp"

SolutionJSSP::SolutionJSSP(const InstanceJSSP& inst, std::mt19937& rng){
    generateRandomSolution(inst,rng);
}

SolutionJSSP::SolutionJSSP(std::vector<int> order):
order(order){}

void SolutionJSSP::generateRandomSolution(const InstanceJSSP& inst, std::mt19937& rng) {
    std::vector<int> jobs;
    jobs.reserve(inst.getNumOps());
    
    int n = inst.getN();
    int m = inst.getM();
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) jobs.push_back(i);

    std::shuffle(jobs.begin(), jobs.end(), rng);

    std::vector<int> counter(n, 0);
    order.reserve(jobs.size());

    for (int i : jobs) 
        order.push_back(inst.getOpId(i, counter[i]++));
}
