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

    Bird& bird = *flock[i];
    candidates.clear();

    int own = (i == 0) ? params.k : params.k - params.x;
    for (int i = 0; i < own; ++i) 
        candidates.push_back(bird.solution->proposeNeighbor(rng));
    
    evals += own;

    int following = frontOf(i);
    if (following != -1) {
        const auto& shared = flock[following]->leftovers;
        int h = std::min(params.x, (int)shared.size());
        for (int s = 0; s < h; ++s) 
            candidates.push_back(shared[s]);
    }

    std::sort(candidates.begin(), candidates.end());

    int firstLeft = 0;
    if (!candidates.empty() && candidates.front().cost < bird.solution->getCost()) {
        const Candidate& c = candidates.front();
        bird.solution = c.parent->materializePropose(c.move,c.cost);

        if (bird.solution->getCost() < best->getCost()) 
            best = bird.solution;
        
        firstLeft = 1;
    }

    auto begin = candidates.begin() + firstLeft;
    int keep = std::min(params.x, (int)candidates.size() - firstLeft);
    bird.leftovers.assign(std::make_move_iterator(begin), std::make_move_iterator(begin + keep));
}