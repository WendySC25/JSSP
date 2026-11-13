#pragma once

#include <iostream>
#include <optional>
#include <string>

struct Args {
    std::string instancePath;
    std::string outPath;
    std::string label = "MBO";

    int nRuns   = 1;         
    int seed    = 1;
    int threads = 0;

    int n = 51;                 
    int k = 5;                  
    int x = 1;                  
    int t = 1000;              
    int K = 1000000;
};

inline std::optional<Args> parseArgs(int argc, char** argv) {
    Args a;
    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if(i + 1 >= argc) {
            std::cerr << "Mising value " << arg << "\n";
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
            else if (arg == "-n")        a.n = std::stoi(v);
            else if (arg == "-k")        a.k = std::stoi(v);
            else if (arg == "-x")        a.x = std::stoi(v);
            else if (arg == "-t")        a.t = std::stoi(v);
            else if (arg == "-K")        a.K = std::stoi(v);
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