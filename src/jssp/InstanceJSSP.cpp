#include "InstanceJSSP.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <numeric>
#include <algorithm>

InstanceJSSP::InstanceJSSP(int n, int m, int lb, int ub):
n(n),m(m),numOps(n*m), lower_bound(lb), upper_bound(ub), machineOf(n*m,0), timeOf(n*m,0), opsPerMachine(m){}

std::shared_ptr<const InstanceJSSP> InstanceJSSP::loadFromFile(const std::string& filepath){
    
    std::ifstream file(filepath);

    if(!file.is_open()) 
        throw std::runtime_error("Useless path: " + filepath);
    
    int n, m, lb, ub;
    if (!(file >> n >> m >> ub >> lb)) 
        throw std::invalid_argument("Missing n m cu ci.");

    std::shared_ptr<InstanceJSSP> instance = std::make_shared<InstanceJSSP>(n, m, lb, ub);

    int mi, ti;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if (!(file >> mi >> ti)) 
                throw std::invalid_argument("Mssing m_i t_i.");

            if (mi < 0 || mi >= m) 
                throw std::out_of_range("Machine index out or range.");
        
            int id = instance->getOpId(i,j); 
            instance->machineOf[id] = mi;
            instance->timeOf[id] = ti;
            instance->opsPerMachine[mi].push_back(id);
        }

    }

    instance->lower_bound = instance->calculateLowerBound();
    instance->upper_bound = instance->calculateUpperBound();

    return instance;
}


int InstanceJSSP::calculateUpperBound() const {
    std::vector<int> jobsUpperBound(n,0);
    std::vector<int> machinesUpperBound(m,0);

    int upper = -1;

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            int id = getOpId(j,i);
            int ma = machineOf[id];
            int t = timeOf[id];

            int initTime = std::max(jobsUpperBound[j], machinesUpperBound[ma]);
            int endTime = initTime + t;

            jobsUpperBound[j] = endTime;
            machinesUpperBound[ma] = endTime;

            upper = std::max(jobsUpperBound[j], upper);
        }
    }

    return upper;
}

int InstanceJSSP::calculateLowerBound() const{

    int maxJobs = 0;
    for(int i = 0; i < n; i++){
        int sumJob = 0;
        for(int j = 0; j < m; j++)
            sumJob += timeOf[getOpId(i,j)];
        maxJobs = std::max(maxJobs, sumJob);
    }

    std::vector<int> temp(m,0);
    int maxMachines = 0;
    for (int i = 0; i < n * m; i++) {
        temp[machineOf[i]] += timeOf[i];
        maxMachines = std::max(maxMachines, temp[machineOf[i]]);
    }

    return std::max(maxMachines,maxJobs);
    
}