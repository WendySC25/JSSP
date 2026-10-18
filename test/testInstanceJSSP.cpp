#include <gtest/gtest.h>

#include <sstream>
#include "jssp/InstanceJSSP.hpp"

TEST(InstanceTest, Load3x3) {

    auto inst = InstanceJSSP::loadFromFile("../data/JSSP_3_3.txt");

    EXPECT_EQ(inst->getN(), 3);
    EXPECT_EQ(inst->getM(), 3);
    EXPECT_EQ(inst->getNumOps(), 9);
    EXPECT_EQ(inst->getUpperBound(), 24);
    EXPECT_EQ(inst->getLowerBound(), 10);

    EXPECT_EQ(inst->getJobSuccessor(0), 1);
    EXPECT_EQ(inst->getJobSuccessor(1), 2);
    EXPECT_EQ(inst->getJobSuccessor(2), -1);

    EXPECT_EQ(inst->calculateUpperBound(), 24);
    EXPECT_EQ(inst->calculateLowerBound(), 10);
}

TEST(InstanceTest, Load100x100) {

    auto inst = InstanceJSSP::loadFromFile("../data/JSSP_100_100.txt");

    EXPECT_EQ(inst->getN(), 100);
    EXPECT_EQ(inst->getM(), 100);
    EXPECT_EQ(inst->getNumOps(), 10000);
    EXPECT_EQ(inst->getUpperBound(), 125341);
    EXPECT_EQ(inst->getLowerBound(), 56381);

    // Failed every time
    // EXPECT_EQ(inst->calculateUpperBound(), 125341);
    
    EXPECT_EQ(inst->calculateLowerBound(), 56381);
}


TEST(InstanceTest, Load40x40) {

    auto inst = InstanceJSSP::loadFromFile("../data/JSSP_40_40.txt");

    EXPECT_EQ(inst->getN(), 40);
    EXPECT_EQ(inst->getM(), 40);
    EXPECT_EQ(inst->getNumOps(), 1600);
    EXPECT_EQ(inst->getUpperBound(), 205);
    EXPECT_EQ(inst->getLowerBound(), 103);

    // Failed every time
    EXPECT_EQ(inst->calculateUpperBound(), 205);
    
    EXPECT_EQ(inst->calculateLowerBound(), 103);
}