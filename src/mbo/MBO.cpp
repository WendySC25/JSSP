#include "MBO.hpp"

int MBO::frontOf(int i) {
    if (i == 0) return -1;
    return i <=2 ? 0 : i - 2;
}

MBO::MBO(std::shared_ptr<const Solution> sol, MBOconfig params):
sol(std::move(sol)), params(params){
    params.validate();
}

MBOResult MBO::run(std::uint32_t seed){

    rng.seed(seed);
    initFlock();

    while (evals < params.K) {
        for(int tour = 0; tour < params.t && evals < params.K; tour++) 
            for(int i = 0; i < params.n && evals < params.K; i++) 
                step(i);

        std::rotate(flock.begin(), flock.begin() + 1, flock.end());
    }

    return MBOResult{best};
}


void MBO::initFlock(){
    flock.clear();
    best = nullptr;
    evals = 0;

    for(int i = 0; i < params.n; i++){
        auto bird = std::make_unique<Bird>();
        bird->solution = sol->createRandom(rng);

        if (!best || bird->solution->getCost() < best->getCost()) 
            best = bird->solution;

        flock.push_back(std::move(bird));
    }

}

void MBO::step(int i) {
}