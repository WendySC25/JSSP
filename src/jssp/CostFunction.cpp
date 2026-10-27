#include "CostFunction.hpp"
#include <iostream>

CostFunction::CostFunction(std::shared_ptr<const InstanceJSSP> inst):
instance(inst),
N(inst->getNumOps()),
m(inst->getM()),
n(inst->getN()),
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
        int base = machine * n;
        int cur = at(base);

        for(int p = 0; p<n; p++) {
            finishT[cur] = 0;
            indegree[cur] = baseIndegree[cur] + (p != 0);

            int nxt = (p + 1 < n) ? at(base + p + 1) : -1;
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


    // double lb = static_cast<double>(instance->getLowerBound());
    // double ub = static_cast<double>(instance->getUpperBound());

    double cost = static_cast<double>(makespan);
    if(cycleNodes > 0) 
        cost += (1000 * cycleNodes);

    return {cost, makespan, cycleNodes};
}


int CostFunction::queueStep(int id, int next, int queueLastIndex) {

    int t = finishT[id] + instance->getTimeOf(next);
    if (t > finishT[next]) finishT[next] = t;
    indegree[next]--;
    if (indegree[next] == 0) queue[queueLastIndex++] = next;

    return queueLastIndex;
}

void CostFunction::dfs(int v, std::vector<std::vector<int> > &components, std::vector<int> &roots, std::vector<int> &t_low, std::vector<int> &t_in, int &timer, std::vector<int> &st) {
    t_low[v] = t_in[v] = timer++;
    st.push_back(v);
 
    std::vector<int> vecinos;
    if(instance->getJobSuccessor(v) != -1) vecinos.push_back(v+1);
    if(nextOpInMachine[v] != -1) vecinos.push_back(nextOpInMachine[v]);
 
    for(int u : vecinos){
        if (t_in[u] == -1) {
            dfs(u, components, roots, t_low, t_in, timer, st);
            t_low[v] = std::min(t_low[v], t_low[u]);
        } else if (roots[u] == -1)
            t_low[v] = std::min(t_low[v], t_in[u]);
    }
 
    if (t_low[v] == t_in[v]) {
        components.push_back({v});
 
        while (true) {
            int u = st.back();
            st.pop_back();
            roots[u] = v;
            if (u == v) break;
            components.back().push_back(u);
        }
    }
}
 
int CostFunction::countCycleNodes(const std::vector<int>& order) {
    initEvaluation(order, -1, -1);
 
    std::vector<int> st;
    std::vector<int> roots(N, -1);
    std::vector<int> t_in(N, -1);
    std::vector<int> t_low(N, -1);
 
    int timer = 0;
 
    std::vector<std::vector<int> > components;
 
    for (int v = 0; v < N; v++) {
        if (t_in[v] == -1)
            dfs(v, components, roots, t_low, t_in, timer, st);
    }
 
    int cyclesNodes = 0;
    for (auto &c : components) {
        if (c.size() > 1) cyclesNodes += c.size();
    }
 
    return cyclesNodes;
}