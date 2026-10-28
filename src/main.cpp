#include <iostream>

#include "jssp/InstanceJSSP.hpp"
#include "exp/ExperimentRunner.hpp"
#include "exp/ExperimentReporter.hpp"

int main() {

    try {
        std::string instancePath = "./data/JSSP_40_40.txt";
        auto instance = InstanceJSSP::loadFromFile(instancePath);

        ExperimentRunner runner(instance);
        RunConfig baseConfig;
    
        baseConfig.label = "REFACTOR_NOMEJOR_40_40_51aves_1M_inicio_factible";
        MBOconfig params;
        params.n = 51;
        params.k = 10;
        params.t = 10000000;
        params.K = 1000000;
        baseConfig.params = params;

        std::vector<RunConfig> configs = runner.assignSeeds(baseConfig, 1, 100);

        std::cout << "Running \n";
        std::vector<RunResult> results = runner.run(configs, 0);

        std::cout << "End!\n";
        ExperimentReporter reporter;
        reporter.generateReportCSV(runner.nowTimestamp() + baseConfig.label +".csv", results);

        int makespan = 10000000;
    
        for(auto r : results) makespan = std::min(makespan, r.makespan);
        std::cout << "Makespan: " << makespan << '\n';

        std::cout << "CVS ready'!\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}