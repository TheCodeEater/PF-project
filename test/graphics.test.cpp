#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../include/graphics.hpp"

namespace ps=particleSimulator;

TEST_CASE("Testing dotted line creation"){
    ps::dottedLine l{{0,0},{150,150}};//create dotted line

    //check extremes
    auto e=l.getExtremes();

    CHECK(e.first==sf::Vector2f{0,0});
    CHECK(e.second==sf::Vector2f{150,150});
}