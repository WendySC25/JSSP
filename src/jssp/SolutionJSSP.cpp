#include "jssp/SolutionJSSP.hpp"

SolutionJSSP::SolutionJSSP(std::shared_ptr<const InstanceJSSP> inst, std::shared_ptr<CostFunction> costFunc, std::mt19937& rng) : 
Solution(0.0),
instance(std::move(inst)),
costFunction(std::move(costFunc)),
order(generateRandomOrder(*instance, rng)) {
    cost = costFunction->evaluate(order);
}

SolutionJSSP::SolutionJSSP(std::shared_ptr<const InstanceJSSP> inst, std::shared_ptr<CostFunction> costFunc, std::vector<int> order, double cost) :
Solution(cost), 
instance(std::move(inst)),
costFunction(std::move(costFunc)),
order(std::move(order)) {}

std::shared_ptr<const Solution> SolutionJSSP::createRandom(std::mt19937& rng) const {
    return std::make_shared<const SolutionJSSP>(instance, costFunction, rng);
}

std::vector<int> SolutionJSSP::generateRandomOrder(const InstanceJSSP& inst, std::mt19937& rng) {
    std::vector<int> jobs;
    jobs.reserve(inst.getNumOps());
    
    int n = inst.getN();
    int m = inst.getM();
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) jobs.push_back(i);

    std::shuffle(jobs.begin(), jobs.end(), rng);

    std::vector<int> counter(n, 0);
    std::vector<int> order;
    order.reserve(jobs.size());

    for (int i : jobs) 
        order.push_back(inst.getOpId(i, counter[i]++));

    return order;
}

std::pair<Move, double> SolutionJSSP::proposeNeighbor(std::mt19937& rng) const {
    Move m;
    // ahhhhh
    return {m, 0.0};
}

std::shared_ptr<const Solution> SolutionJSSP::materializePropose(const Move& move, double newCost) const {
    std::vector<int> nextOrder = order;
    std::swap(nextOrder[move.i], nextOrder[move.j]);
    return std::make_shared<const SolutionJSSP>(instance, costFunction, std::move(nextOrder), newCost);
}

std::string SolutionJSSP::toString(){
    std::string s;
    for(int id : order) s += std::to_string(id) + " ";
    return s;
}