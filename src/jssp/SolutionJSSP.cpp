#include "jssp/SolutionJSSP.hpp"

SolutionJSSP::SolutionJSSP(std::shared_ptr<CostFunction> costFunc, std::mt19937& rng) : 
Solution(0.0),
costFunction(std::move(costFunc)),
order(generateRandomOrder(costFunction->getInstance(), rng)) {
    cost = costFunction->evaluate(order);
}

SolutionJSSP::SolutionJSSP(std::shared_ptr<CostFunction> costFunc, std::vector<int> ord, double cost):
Solution(cost),
costFunction(std::move(costFunc)),
order(std::move(ord)) {}

std::shared_ptr<const Solution> SolutionJSSP::createRandom(std::mt19937& rng) const {
    return std::make_shared<const SolutionJSSP>(costFunction, rng);
}

std::vector<int> SolutionJSSP::generateRandomOrder(const InstanceJSSP& inst, std::mt19937& rng) {
    std::vector<int> jobs;
    jobs.reserve(inst.getNumOps());
    
    int n = inst.getN();
    int m = inst.getM();
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) jobs.push_back(i);

    std::shuffle(jobs.begin(), jobs.end(), rng);

    std::vector<int> nextOp(n, 0);
    std::vector<int> fill(m, 0); 
    std::vector<int> order(inst.getNumOps());

    for (int j : jobs) {
        int id = inst.getOpId(j, nextOp[j]++);
        int mach = inst.getMachineOf(id);
        order[mach * n + fill[mach]++] = id;
    }
    return order;
}

Candidate SolutionJSSP::proposeNeighbor(std::mt19937& rng) const {

    const InstanceJSSP& inst = costFunction->getInstance();
    const int n = inst.getN();

    std::uniform_int_distribution<int> machineDist(0, inst.getM() - 1);
    
    std::uniform_int_distribution<int> opDist(0, n - 1);

    int a = opDist(rng);
    int b = opDist(rng);
    while (a == b) 
        b = opDist(rng);

    int base = machineDist(rng) * n;
    Move mv{base + a, base + b};
    double neighborCost = costFunction->evaluateSwap(order, mv.i, mv.j);
    return Candidate{shared_from_this(), mv, neighborCost};
}

std::shared_ptr<const Solution> SolutionJSSP::materializePropose(const Move& move, double newCost) const{

    std::vector<int> nextOrder = order;
    std::swap(nextOrder[move.i], nextOrder[move.j]);

    return std::make_shared<const SolutionJSSP>(costFunction, std::move(nextOrder), newCost);
}

std::string SolutionJSSP::toString() const {
    int n = costFunction->getInstance().getN();
    int m = costFunction->getInstance().getM();

    std::string s;
    for(int machine = 0; machine < m; machine++){
        s += "[";
        for(int i = 0; i < n; i++) {
            if(i) s += ", ";
            s += std::to_string(order[machine * n + i]);
        }
        s += "]\n";
    }
    return s;
}