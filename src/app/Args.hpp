#pragma once

#include <iostream>
#include <optional>
#include <string>

#include "mbo/MBOConfig.hpp"

enum class ExecutionMode {
    /** Run a single experiment with the provided parameters. */
    Single,

    /** Run N experiments with random hyperparameters. */
    Random,

    /** Run N experiments with the same parameters but different random seeds. */
    Seeds
};


struct Args {
    std::string instancePath;
    std::string outPath;
    std::string label = "MBO";
    ExecutionMode mode = ExecutionMode::Single;

    int nRuns   = 1;         
    int seed    = 1;
    int threads = 0;

    MBOconfig params {
        .n = 51,
        .k = 5,
        .x = 1,
        .t = 1000,
        .K = 1000000
    };
};

inline std::optional<Args> parseArgs(int argc, char** argv) {
    Args a;
    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if(i + 1 >= argc) {
            std::cerr << "Missing value " << arg << "\n";
            return std::nullopt;
        }

        std::string v = argv[++i];
        try {
            if      (arg == "-instance") a.instancePath = v;
            else if (arg == "-out")      a.outPath = v;
            else if (arg == "-label")    a.label = v;
            else if (arg == "-nRuns")    a.nRuns = std::stoi(v);
            else if (arg == "-seed")     a.seed = std::stoi(v);
            else if (arg == "-threads")  a.threads = std::stoi(v);
            else if (arg == "-n")        a.params.n = std::stoi(v);
            else if (arg == "-k")        a.params.k = std::stoi(v);
            else if (arg == "-x")        a.params.x = std::stoi(v);
            else if (arg == "-t")        a.params.t = std::stoi(v);
            else if (arg == "-K")        a.params.K = std::stoi(v);
            else if (arg == "-mode"){
                if (v == "random")       a.mode = ExecutionMode::Random;
                else if (v == "seeds")   a.mode = ExecutionMode::Seeds;
                else                     a.mode = ExecutionMode::Single;
            }
            else {
                std::cerr << "IDK: " << arg << "\n";
                return std::nullopt;
            }
        } catch (const std::exception&) {
            std::cerr << "Invalid " << arg << ": " << v << "\n";
            return std::nullopt;
        }
    }

    if(a.instancePath.empty()) {
        std::cerr << "Missing JSSP instance\n";
        return std::nullopt;
    }
    
    return a;
}