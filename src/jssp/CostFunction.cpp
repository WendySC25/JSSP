#include "CostFunction.hpp"

CostFunction::CostFunction(std::shared_ptr<const InstanceJSSP> inst):
instance(inst),
N(inst->getNumOps()),
m(inst->getM()),
indegree(N),
baseIndegree(N),
queue(N),
finishT(N),
nextOpInMachine(N){
    for (int id = 0; id < N; id++) {
        int i = instance->getJobSuccessor(id);
        if (i != -1) baseIndegree[i]++;
    }
}

void CostFunction::initEvaluation(const std::vector<int>& order, int i, int j) {
    
    auto at = [&](int p) {
        if (p == i) return order[j];
        if (p == j) return order[i];
        return order[p];
    };

    for(int machine = 0; machine<m; machine++) {
        int base = machine * m;
        int cur = at(base);

        for(int p = 0; p<m; p++) {
            finishT[cur] = 0;
            indegree[cur] = baseIndegree[cur] + (p != 0);

            int nxt = (p+1 < m) ? at(base+p+1) : -1;
            nextOpInMachine[cur] = nxt;
            cur = nxt;
        }
    }
}


std::pair<int, int> CostFunction::runKanh(){

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

Evaluation CostFunction::evaluateDetailed(const std::vector<int>& order){
    initEvaluation(order,-1,-1);
    return calculateEvaluation();
}

Evaluation CostFunction::evaluateSwapDetailed(const std::vector<int>& order, int i, int j){
    initEvaluation(order,i,j);
    return calculateEvaluation();
}

Evaluation CostFunction::calculateEvaluation() {
    auto [makespan,cycleNodes] = runKanh();

    double lb = static_cast<double>(instance->getLowerBound());
    double ub = static_cast<double>(instance->getUpperBound());

    double cost = (static_cast<double>(makespan) - lb) / std::max(1.0, ub - lb);
    cost += (lb * cycleNodes);
    
    return {cost, makespan, cycleNodes};
}


int CostFunction::queueStep(int id, int next, int queueLastIndex) {

    int t = finishT[id] + instance->getTimeOf(next);
    if (t > finishT[next]) finishT[next] = t;
    indegree[next]--;
    if (indegree[next] == 0) queue[queueLastIndex++] = next;

    return queueLastIndex;
}
