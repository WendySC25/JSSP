#include <iostream>

#include "jssp/InstanceJSSP.hpp"
#include "jssp/SolutionJSSP.hpp"
#include "mbo/MBO.hpp"

int main() {
    
    auto inst = InstanceJSSP::loadFromFile("./data/JSSP_40_40.txt");
    
    MBOconfig config;
    config.n = 51;
    config.k = 5;
    config.x = 1;
    config.t = 10;
    config.K = 1000;

    int seed = 25;
    std::mt19937 rng(seed);

    auto costFunc = std::make_shared<CostFunction>(inst);
    auto solution = std::make_shared<const SolutionJSSP>(inst, costFunc, rng);
    MBO mbo(solution, config);
    MBOResult res = mbo.run(seed);

    const auto& bestJSSP = static_cast<const SolutionJSSP&>(*res.bestSolution);

    Evaluation eval = costFunc->evaluateDetailed(bestJSSP.getOrder());
    std::cout << "Makespan:     " << eval.makespan << std::endl;
    std::cout << "Nodos Ciclos: " << eval.cycleNodes << std::endl;
    std::cout << "Costo:        " << eval.cost << std::endl;
    std::cout << "Solución:   \n" << bestJSSP.toString() << std::endl;

    return 0;
}