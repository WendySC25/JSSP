#include <iostream>

#include "app/App.hpp"
#include "app/Args.hpp"

inline std::string usage() {
    return
    R"(Usage: ./jssp_runner -instance <path> [options]

    -instance <path>   Instance file (required)
    -out <path>        Output CSV (default ./data/output/<timestamp>_<label>.csv)
    -label <text>      Experiment label (default MBO)
    -nRuns <N>         Number of runs, with seeds seed, seed+1, ... (default 1)
    -seed <S>          Seed (default 1)
    -threads <N>       Threads (0 = automatic)

    -n <birds>  -k <neighbors>  -x <shared>  -t <tours>  -K <evaluations>
    )";
}

int main(int argc, char** argv) {
    try {
        auto opts = parseArgs(argc, argv);

        if(!opts) {
            std::cerr << "\n" << usage();
            return 1;
        }

        App app;
        return app.run(*opts);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}