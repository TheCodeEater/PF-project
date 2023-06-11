#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "../include/particleGenerator.hpp"
#include "../include/traiettoria.hpp"


TEST_CASE("Test della generazione in range dei numeri"){
    const float r1_{400}; //parametro r1

    particleSimulator::randSimulator rs{0.f,90.f,0.f,pi/4,r1_};

    for(int i{};i<1e6;++i){
        particleSimulator::particle p{rs()};

        CHECK(p.pos.y()<=r1_);
        CHECK(p.pos.y()>=-r1_);
    }

}