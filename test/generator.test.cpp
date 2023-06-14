#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "../include/particleGenerator.hpp"
#include "../include/traiettoria.hpp"

#include <fstream>

TEST_CASE("Test della generazione in range dei numeri"){
    const float r1{400}; //parametro r1
    const float r2{200};
    const float l{700};
    const unsigned seed{std::random_device{}()};

    particleSimulator::path biliardo{r1,r2,l};

    const unsigned seed{std::random_device{}()};
    std::cout<<"Seed: "<<seed<<"\n";

    particleSimulator::randSimulator rs{{r1,r2,l,0.f,80.f,0.f,pi/8,seed}};


    std::vector<particleSimulator::exit_point> exit{};

    try{
        exit=rs.run(100000,200);
        std::ofstream f{"output.txt"};
        for(auto const& v:exit){
            f<<v.theta<<"\n";
        }
    }catch(std::exception const& e){
        std::cout<<e.what()<<"\n";
        std::cout<<"Seed incriminato: "<<seed<<"\n";
        throw;
    }

<<<<<<< HEAD
    particleSimulator::particle bad_p=rs.getParticle();

    std::cout<<bad_p.pos<<"\n"<<bad_p.theta<<"\n";*/

    std::vector<particleSimulator::exit_point> exit=rs.run(1000000,200);

   // auto p=rs.getParticle();
    //std::cout<<p.pos<<"\n"<<p.theta<<"\n";

    std::ofstream f{"output.txt"};

    for(auto const& v:exit){
        f<<v.theta<<"\n";
    }
=======
>>>>>>> 391398d0e3185f86ed3f0e0261765c5c3fa0f0b9

}