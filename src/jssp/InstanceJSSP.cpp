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
    if (!(file >> n >> m >> lb >> ub)) 
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

    return instance;
}

