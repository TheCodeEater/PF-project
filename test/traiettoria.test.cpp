#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "../include/traiettoria.hpp"

TEST_CASE("test traiettoria con Eigen"){

    particleSimulator::path biliardo{Line::Through({0,10},{100,5}), Line::Through({0,-10},{100,-5})}; //oggetto di calcolo della traiettoria

    SUBCASE("Collisioni con il bordo superiore"){
        particleSimulator::particle p{{0,0}, std::atan(1)}; //particella con angolo di 45 gradi nell'origine
        const Eigen::Vector2f intersection = biliardo(p);
        
        CHECK(intersection.x()==doctest::Approx(9.5238));
    }

}