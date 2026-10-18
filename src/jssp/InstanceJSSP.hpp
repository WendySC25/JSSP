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


    public:
        InstanceJSSP(int n, int m, int lb, int ub);

        static std::shared_ptr<const InstanceJSSP> loadFromFile(const std::string& filepath);

        inline int getOpId(int job, int order) const { 
            return job * m + order;
        }

        inline int getJobSuccessor(int opId) const {
            return ((opId % m) < (m - 1)) ? (opId + 1) : -1;
        }

        int calculateUpperBound() const;
        int calculateLowerBound() const;

        /** Getters */
        inline int getN() const { return n; }
        inline int getM() const { return m; }
        inline int getNumOps() const { return numOps; }
        inline int getLowerBound() const { return lower_bound; }
        inline int getUpperBound() const { return upper_bound; }

        inline const std::vector<int>& getMachines() const { return machineOf; }
        inline const std::vector<int>& getTimes() const { return timeOf; }
        inline const std::vector<std::vector<int>>& getOpsPerMachines() const { return opsPerMachine; }
        
        inline int getMachineOf(int opId) const { return machineOf[opId]; }
        inline int getTimeOf(int opId) const { return timeOf[opId]; }
        inline const std::vector<int>& getOpsForMachine(int machineId) const { return opsPerMachine[machineId]; }

};