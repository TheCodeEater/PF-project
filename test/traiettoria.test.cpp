#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "../include/traiettoria.hpp"
#include <iostream>

TEST_CASE("test traiettoria con Eigen"){

    const int r1=10;
    const int r2=5;
    const int l=100;
    particleSimulator::path biliardo{r1,r2,l}; //oggetto di calcolo della traiettoria

    SUBCASE("Collisioni con il bordo superiore"){
        particleSimulator::particle p{{0,0}, std::atan(1)}; //particella con angolo di 45 gradi nell'origine
        const Eigen::Vector2f intersection = biliardo(p);
        
        CHECK(intersection.x()==doctest::Approx(9.5238));
    }
}
TEST_CASE("Test dell'orientazione del vettore"){
        const int r1=10;
        const int r2=5;
        const int l=100;
        particleSimulator::path biliardo{r1,r2,l};

        SUBCASE("Particella 1"){
            const particleSimulator::particle p{{0,0}, particleSimulator::path::arctan(-1,-1)};
            const Eigen::Vector2f intersection = biliardo(p);

            CHECK(biliardo.getHitDirection(p.theta)==particleSimulator::vecOrientation::Down);
        }
        SUBCASE("Particella 2"){
            const particleSimulator::particle p1{{0,0}, std::atan(0)};
            const Eigen::Vector2f intsect=biliardo(p1);
            CHECK(biliardo.getHitDirection(p1.theta)==particleSimulator::vecOrientation::Horizontal);
        }
        SUBCASE("Particella 3"){
            const particleSimulator::particle p2{{0,5}, particleSimulator::path::arctan(0,-1)};
            const Eigen::Vector2f intsect=biliardo(p2);

            std::cout<<intsect<<"\n";

            CHECK(biliardo.getHitDirection(p2.theta)==particleSimulator::vecOrientation::Horizontal);
            CHECK(biliardo.getLocationType(intsect)==particleSimulator::posTypes::BackHit);
        }
        
    }