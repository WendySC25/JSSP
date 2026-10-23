#include <gtest/gtest.h>

#include <sstream>
#include "jssp/CostFunction.hpp"
#include "jssp/InstanceJSSP.hpp"

static const std::vector<int> good = {0, 3, 6, 1, 4, 7, 2, 5, 8};
static const std::vector<int> cycle = {7, 0, 1, 6, 3, 4, 5, 2, 8};

auto inst = InstanceJSSP::loadFromFile("../data/JSSP_3_3.txt");

TEST(CostFuntionTest, JSSP3x3Factible){
    CostFunction f(inst);
    Evaluation e = f.evaluateDetailed(good);

    EXPECT_EQ(e.makespan, 12);
    EXPECT_EQ(e.cycleNodes, 0);

}

TEST(CostFunctionTest, Cycle3x3Infeasible) {
    CostFunction f(inst);
    Evaluation e = f.evaluateDetailed(cycle);
    EXPECT_GT(e.cycleNodes, 0);          
}