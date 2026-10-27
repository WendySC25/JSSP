#include "exp/ExperimentRunner.hpp"

ExperimentRunner::ExperimentRunner(std::shared_ptr<const InstanceJSSP> inst):
instance(inst) {}

std::string ExperimentRunner::nowTimestamp() const{
    auto noww = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(noww);
    std::tm tmBuf;
    #if defined(_WIN32)
        localtime_s(&tmBuf, &t);
    #else
        localtime_r(&t, &tmBuf);
    #endif
    std::ostringstream oss;
    oss << std::put_time(&tmBuf, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}

RunResult ExperimentRunner::runOne(const RunConfig& config) const {
    RunResult res;
    res.config = config;
    res.cost = -1;

    res.timestamp = nowTimestamp();

    try {
        std::mt19937 randSeed(config.seed);
        auto cf = std::make_shared<CostFunction>(instance);
        auto solutionJSSP = std::make_shared<const SolutionJSSP>(cf, randSeed);

        MBO mbo(solutionJSSP, config.params);
        auto t0 = std::chrono::steady_clock::now();
        MBOResult r = mbo.run(config.seed);
        auto t1 = std::chrono::steady_clock::now();

        res.seconds = std::chrono::duration<double>(t1 - t0).count();
        res.solutionString = r.bestSolution->toString();
        res.cost = r.bestSolution->getCost();
        res.ok = true;
        
    }catch(const std::exception& e) {
        res.error = e.what();
    }catch(...) {
        res.error = "error ?";
    }

    return res;
}

std::vector<RunResult> ExperimentRunner::run(const std::vector<RunConfig>& configs, int threads) const {
    int total = configs.size();
    std::vector<RunResult> results(total);

    if(threads == 0)
        threads = std::thread::hardware_concurrency();

    if(threads == 0) 
        threads = 1;

    threads = std::min(threads, total);

    std::atomic<std::size_t> next{0};

    auto worker = [&]() {
        while (true) {
            int i = next.fetch_add(1);
            if(i >= total) break;

            results[i] = runOne(configs[i]);
        }
    };

    if(threads == 1)
        worker();
    else {
        std::vector<std::thread> pool;
        pool.reserve(threads);
        for(int t = 0; t<threads; t++) 
            pool.emplace_back(worker);

        for(auto& th : pool) 
            th.join();
    }
    return results;
}

std::vector<RunConfig> ExperimentRunner::assignSeeds(const RunConfig& base, int runs, int baseSeed) {
    std::vector<RunConfig> configs;
    configs.reserve(runs);

    for(int r = 0; r<runs; r++) {
        RunConfig s = base;
        s.seed = baseSeed + r;
        configs.push_back(std::move(s));
    }
    return configs;
}

