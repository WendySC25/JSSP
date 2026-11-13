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

        auto jsspSol = std::dynamic_pointer_cast<const SolutionJSSP>(r.bestSolution);
        if(jsspSol) {
            Evaluation eval = cf->evaluateDetailed(jsspSol->getOrder());
            res.makespan = eval.makespan;   
            res.nodesCycles = eval.cycleNodes;
        }
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

std::vector<RunConfig> ExperimentRunner::generateRandomConfigs(int numConfigs, int baseSeed) { 

    std::mt19937 rng(baseSeed);
    std::uniform_int_distribution<int> distHalfN(5, 50);
    std::uniform_int_distribution<int> distX(1, 3);
    std::uniform_int_distribution<int> distk(6, 10);
    std::uniform_int_distribution<int> distK(500000, 1000000);
    std::uniform_int_distribution<int> distSeed(1, 1000000);
 
    std::vector<RunConfig> configs;
    configs.reserve(numConfigs);
 
    for(int i = 0; i < numConfigs; i++) {
        MBOconfig params;

        params.n = 2 * distHalfN(rng) + 1;
        params.x = distX(rng);
        params.k = 2 * params.x + 1 + distk(rng);
        params.K = distK(rng);

        int perTour = params.k + (params.n - 1) * (params.k - params.x);
        int tMax = std::max(1, params.K / (perTour * 3));
        params.t = std::uniform_int_distribution<int>(1, tMax)(rng);
 
        params.validate();
 
        RunConfig config;
        config.label = "Random_MBO_Run_" + std::to_string(i + 1);
        config.params = params;
        config.seed = distSeed(rng);
        configs.push_back(config);
    }

    return configs;
}
