#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

class InstanceJSSP {
    private:
        /** Number of jobs */
        int n;

        /** Number of machines */
        int m;

        /** Number of operations */
        int numOps;

        /** max(max job total, max machine total) */
        int lower_bound = 0;

        /** The sequential time of all jobs */
        int upper_bound = 0;

        /** Machine assign to operation i*/
        std::vector<int> machineOf;

        /** Duration time of operation i */
        std::vector<int> timeOf;

        std::vector<std::vector<int> > opsPerMachine;

        int calculateUpperBound();
        int calculateLowerBound();

    public:
        InstanceJSSP(int n, int m, int lb, int ub);

        static std::shared_ptr<const InstanceJSSP> loadFromFile(const std::string& filepath);

        inline int getOpId(int job, int order) const { return job * m + order; }
        

};