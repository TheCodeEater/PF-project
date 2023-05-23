#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "../include/traiettoria.hpp"

TEST_CASE("test traiettoria con Eigen"){
    particleSimulator::particle p{{0,0}, 22/7/4};
    Line borderup{Line::Through({0,10},{100,5})};
    //Line borderdown;
    particleSimulator::path pluto{borderup, Line::Through({0,0},{0,0})};
    Eigen::Vector2f intersection = pluto(p);
    CHECK(intersection.x()==doctest::Approx(9.5));

}