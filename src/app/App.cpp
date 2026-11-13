#include "app/App.hpp"

#include <filesystem>
#include <iostream>
#include <vector>

#include "exp/ExperimentReporter.hpp"
#include "exp/ExperimentRunner.hpp"
#include "jssp/InstanceJSSP.hpp"

int App::run(const Args& args) {

    auto instance = InstanceJSSP::loadFromFile(args.instancePath);
    ExperimentRunner runner(instance);

    RunConfig base;
    base.label = args.label;
    base.params = args.params;
    base.randomStart = args.randomStart;
    
    std::vector<RunConfig> allConfigs;
    switch (args.mode) {
        case ExecutionMode::Single:
            allConfigs.push_back(base);
            break;
            
        case ExecutionMode::Random:
            allConfigs = ExperimentRunner::generateRandomConfigs(args.nRuns, args.seed);
            break;
            
        case ExecutionMode::Seeds:
            allConfigs = ExperimentRunner::assignSeeds(base, args.nRuns);
            break;
    }

    std::cout << "Running " << allConfigs.size() << " experiment\n";
    std::vector<RunResult> results = runner.run(allConfigs, args.threads);

    // CSV
    std::string out = args.outPath.empty()
        ? "./data/output/" + runner.nowTimestamp() + "_" + args.label + ".csv"
        : args.outPath;

    std::filesystem::path dir = std::filesystem::path(out).parent_path();
    if(!dir.empty()) std::filesystem::create_directories(dir);

    ExperimentReporter reporter;
    reporter.generateReportCSV(out, results);

    int best = -1;
    for(const auto& r : results)
        if(r.ok && r.nodesCycles == 0 && (best < 0 || r.makespan < best)) best = r.makespan;

    if(best >= 0) std::cout << "Best makespan: " << best << "\n";
    else std::cout << "No factible solutions \n";
    std::cout << "CSV: " << out << "\n";
    return 0;

}