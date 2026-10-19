#include "CostFunction.hpp"

CostFunction::CostFunction(std::shared_ptr<const InstanceJSSP> inst):
instance(inst),
N(inst->getNumOps()),
m(inst->getM()),
indegree(N),
baseIndegree(N),
queue(N),
finishT(N),
lastOpInMachine(m),
nextOpInMachine(N){
    for (int id = 0; id < N; id++) 
        baseIndegree[static_cast<std::size_t>(id)] = (id % m == 0) ? 0 : 1;
}

void CostFunction::calculateIndegree(const std::vector<int>& order){

    for(const int id : order){
        int machine = instance->getMachineOf(id);
        int prev = lastOpInMachine[machine];
        if(prev != -1) {
            nextOpInMachine[prev] = id;
            indegree[id]++;
        } 
        lastOpInMachine[machine] = id;
    }

}

std::pair<int, int> CostFunction::getEvaluationValues(){

    int queueLastIndex = 0;
    for (int id = 0; id < N; ++id) {
        if (indegree[id] == 0) {
            queue[queueLastIndex++] = id;
            finishT[id] = instance->getTimeOf(id);
        }
    }

    int front = 0;
    long long makespan = 0;
    
    while(front < queueLastIndex) {
        int id = queue[front++];
        makespan = std::max(makespan, (long long)finishT[id]);

        if ((id + 1) % m != 0) 
            queueLastIndex = queueStep(id, id + 1, queueLastIndex);

        if (nextOpInMachine[id] != -1) 
            queueLastIndex = queueStep(id, nextOpInMachine[id], queueLastIndex);
    }

    return {makespan, N - queueLastIndex};
}

double CostFunction::evaluate(const std::vector<int>& order){

    std::fill(finishT.begin(), finishT.end(), 0);
    std::fill(lastOpInMachine.begin(), lastOpInMachine.end(), -1);
    std::fill(nextOpInMachine.begin(), nextOpInMachine.end(), -1);
    std::copy(baseIndegree.begin(), baseIndegree.end(), indegree.begin());
    
    calculateIndegree(order);
    
    auto values = getEvaluationValues();
    int makespan = values.first;
    int cycleNodes = values.second;

    double cost = (makespan - instance->getLowerBound()) /  (instance->getUpperBound() - instance->getLowerBound());
    cost += (instance->getLowerBound() * cycleNodes);

    return cost;
}

int CostFunction::queueStep(int id, int next, int queueLastIndex) {

    int t = finishT[id] + instance->getTimeOf(next);
    if (t > finishT[next]) finishT[next] = t;
    indegree[next]--;
    if (indegree[next] == 0) queue[queueLastIndex++] = next;

    return queueLastIndex;
}
