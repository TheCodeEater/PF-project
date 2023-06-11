#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "../include/particleGenerator.hpp"

TEST_CASE("Test della generazione in range dei numeri"){
    const float r1_{400}; //parametro r1

    const particleSimulator::randSimulator rs{0.f,1.f,0.f,pi/4,r1_,10};

    auto pos_gen=rs.getPosGenerator();
    auto ang_gen=rs.getAngleGenerator();

    CHECK(pos_gen.max()<=r1_);
    CHECK(pos_gen.min()>=-r1_);
}