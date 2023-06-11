#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "../include/particleGenerator.hpp"
#include "../include/traiettoria.hpp"

#include <fstream>

TEST_CASE("Test della generazione in range dei numeri"){
    const float r1{400}; //parametro r1
    const float r2{200};
    const float l{700};

    particleSimulator::path biliardo{r1,r2,l};

    particleSimulator::randSimulator rs{0.f,80.f,0.f,pi/4,biliardo};

    /*for(int i{};i<1e3;++i){
        particleSimulator::particle p{rs.getParticle()};

        CHECK(p.pos.y()<=r1);
        CHECK(p.pos.y()>=-r1);
    }*/

    //TEST
    std::vector<particleSimulator::particle> exit=rs.run(1e5,200);

    std::ofstream f{"output.txt"};

    for(auto const& v:exit){
        f<<v.theta<<"\n";
    }

}