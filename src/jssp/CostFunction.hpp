#pragma once

#include "InstanceJSSP.hpp"
#include "Evaluation.hpp"

class CostFunction{
    private:
    
        std::shared_ptr<const InstanceJSSP> instance;

        int N;
        int m;

        std::vector<int> indegree;
        std::vector<int> baseIndegree;
        std::vector<int> queue; 
        std::vector<int> finishT;
        std::vector<int> lastOpInMachine;
        std::vector<int> nextOpInMachine;
        
        int queueStep(int id, int v, int queueLastIndex);

    public:
        CostFunction(std::shared_ptr<const InstanceJSSP> inst);
        
        Evaluation evaluateDetailed(const std::vector<int>& order);
        std::pair<int, int> runKanh();

        void calculateIndegree(const std::vector<int>& order);

        inline double evaluate(const std::vector<int>& order) {
            return evaluateDetailed(order).cost;
        }
        
};
