#pragma once

#include "InstanceJSSP.hpp"
#include "Evaluation.hpp"

class CostFunction{
    private:
    
        std::shared_ptr<const InstanceJSSP> instance;

        int N;
        int m;
        int n;

        std::vector<int> indegree;
        std::vector<int> baseIndegree;
        std::vector<int> queue; 
        std::vector<int> finishT;
        std::vector<int> nextOpInMachine;
        
        int queueStep(int id, int v, int queueLastIndex);
        Evaluation calculateEvaluation();
        void dfs(int v, std::vector<std::vector<int> > &components, std::vector<int> &roots, std::vector<int> &t_low, std::vector<int> &t_in, int &timer, std::vector<int> &st);

    public:
        CostFunction(std::shared_ptr<const InstanceJSSP> inst);

        void initEvaluation(const std::vector<int>& order, int i, int j);
        
        Evaluation evaluateDetailed(const std::vector<int>& order);
        Evaluation evaluateSwapDetailed(const std::vector<int>& order, int i, int j);

        std::pair<int, int> runKanh();

        int countCycleNodes(const std::vector<int>& order);

        inline double evaluate(const std::vector<int>& order) {
            return evaluateDetailed(order).cost;
        }

        inline double evaluateSwap(const std::vector<int>& order, int i, int j) {
            return evaluateSwapDetailed(order, i, j).cost;
        }

        const InstanceJSSP& getInstance() const { return *instance; }
        
};
