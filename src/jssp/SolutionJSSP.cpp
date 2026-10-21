#include "jssp/SolutionJSSP.hpp"

SolutionJSSP::SolutionJSSP(std::shared_ptr<const InstanceJSSP> inst, std::shared_ptr<CostFunction> costFunc, std::mt19937& rng) : 
Solution(0.0),
instance(std::move(inst)),
costFunction(std::move(costFunc)),
order(generateRandomOrder(*instance, rng)) {
    posOfOp.resize(this->order.size());
    for (int id = 0; id < this->order.size(); id++) 
        posOfOp[this->order[id]] = id;
    
    cost = costFunction->evaluate(order);
}

SolutionJSSP::SolutionJSSP(std::shared_ptr<const InstanceJSSP> inst, std::shared_ptr<CostFunction> costFunc, std::vector<int> ord, std::vector<int> pos, double cost):
Solution(cost),
instance(std::move(inst)),
costFunction(std::move(costFunc)),
order(std::move(ord)),
      posOfOp(std::move(pos)) {}

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

Candidate SolutionJSSP::proposeNeighbor(std::mt19937& rng) const {

    std::uniform_int_distribution<int> machineDist(0, instance->getM() - 1);

    int machine = machineDist(rng);
    const std::vector<int>& machineOps = instance->getOpsForMachine(machine);

    std::uniform_int_distribution<int> opDist(0, machineOps.size() - 1);

    int a = opDist(rng);
    int b = opDist(rng);
    while (a == b) 
        b = opDist(rng);
    
    Move mv{posOfOp[machineOps[a]], posOfOp[machineOps[b]]};
    std::swap(order[mv.i], order[mv.j]);
    double neighborCost = costFunction->evaluate(order);

    std::swap(order[mv.i], order[mv.j]);
    return Candidate{shared_from_this(), mv, neighborCost};
}

std::shared_ptr<const Solution> SolutionJSSP::materializePropose(const Move& move, double newCost) const{

    std::vector<int> nextOrder = order;
    std::swap(nextOrder[move.i], nextOrder[move.j]);

    std::vector<int> nextPos = posOfOp;
    nextPos[nextOrder[move.i]] = move.i;
    nextPos[nextOrder[move.j]] = move.j;

    return std::make_shared<const SolutionJSSP>(instance, costFunction, std::move(nextOrder), std::move(nextPos), newCost);
}

std::string SolutionJSSP::toString() const {
    std::vector<std::string> sv(instance->getM(),"[");

    for(int id : order){
        int m = instance->getMachineOf(id);
        sv[m] += std::to_string(id) + ", ";
    }
    
    std::string s;
    for(int i = 0; i < instance->getM(); i++){
        s += sv[i] + "] \n";
    }

    return s;
}