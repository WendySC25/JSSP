#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <iomanip>

#include "jssp/InstanceJSSP.hpp"
#include "jssp/SolutionJSSP.hpp"
#include "mbo/MBO.hpp"
#include "mbo/MBOConfig.hpp"
#include "mbo/MBOResult.hpp"
#include "jssp/CostFunction.hpp"

int main() {
    auto inst = InstanceJSSP::loadFromFile("./data/JSSP_100_100.txt");

    MBOconfig config;
    config.n = 51;
    config.k = 5;
    config.x = 1;
    config.t = 10;
    config.K = 1000000; 

    int numExperiments = 1000;
    std::uint32_t baseSeed = 42;

    int bestMakespanOverall = 1e9;
    double sumMakespan = 0.0;

    std::cout << "Exp\tSeed\tMakespan\tCiclos\n";
    std::cout << "------------------------------------------------\n";

    for (int i = 0; i < numExperiments; ++i) {
        std::uint32_t currentSeed = baseSeed + i * 13;
        std::mt19937 rng(currentSeed);

        auto costFunc = std::make_shared<CostFunction>(inst);
        auto solution = std::make_shared<const SolutionJSSP>(costFunc, rng);

        MBO mbo(solution, config);
        MBOResult res = mbo.run(currentSeed);

        const auto& bestJSSP = static_cast<const SolutionJSSP&>(*res.bestSolution);
        Evaluation eval = costFunc->evaluateDetailed(bestJSSP.getOrder());

        sumMakespan += eval.makespan;
        if (eval.makespan < bestMakespanOverall) 
            bestMakespanOverall = eval.makespan;
        
        std::cout << (i + 1) << "\t" << currentSeed << "\t" << eval.makespan << "\t" << eval.cycleNodes << "\n";
    }

    std::cout << "Makespan Promedio: " << (sumMakespan / numExperiments) << std::endl;
    std::cout << "Makespan Global: " << bestMakespanOverall << std::endl;

    return 0;
}