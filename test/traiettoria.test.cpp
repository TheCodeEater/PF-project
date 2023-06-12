#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../include/traiettoria.hpp"

#include <iostream>

#include "doctest.h"

using pT = particleSimulator::posTypes;

TEST_CASE("Test arcotangente") {
  // alcuni test per la mia arctan
}

TEST_CASE("Test del calcolo delle traiettorie") {
  // dati del biliardo di esempio
  const int r1 = 10;
  const int r2 = 5;
  const int l = 100;

  particleSimulator::path biliardo{r1, r2,
                                   l};  // oggetto di calcolo della traiettoria

  // particelle
  particleSimulator::particle p0{{0, 0}, std::atan(1)};
  particleSimulator::particle p1{{0, 0},
                                 particleSimulator::path::arctan(-1, -1)};
  particleSimulator::particle p2{{0, 0}, std::atan(0)};
  particleSimulator::particle p3{{0, 5},
                                 particleSimulator::path::arctan(0, -1)};

  // Calcolo orientazione particella
  SUBCASE("Test calcolo orientazione traiettoria") {
    CHECK(biliardo.getHitDirection(p0.theta) ==
          particleSimulator::vecOrientation::UpRight);
    CHECK(biliardo.getHitDirection(p1.theta) ==
          particleSimulator::vecOrientation::DownLeft);
    CHECK(biliardo.getHitDirection(p2.theta) ==
          particleSimulator::vecOrientation::HorizontalRight);
    CHECK(biliardo.getHitDirection(p3.theta) ==
          particleSimulator::vecOrientation::HorizontalLeft);
  }

  // calcolo intersezioni
  const Eigen::Vector2f int0 = biliardo(p0).point;
  const Eigen::Vector2f int1 = biliardo(p1).point;
  const Eigen::Vector2f int2 = biliardo(p2).point;
  const Eigen::Vector2f int3 = biliardo(p3).point;

  SUBCASE("Test collisioni dopo il lancio") {
    CHECK(biliardo.getLocationType(int0) == pT::Inside);
    CHECK(biliardo.getLocationType(int1) ==
          pT::BackHit);  // NOTA: sarebbe un colpo nel bordo, ma resta comunque
                         // dentro
    CHECK(biliardo.getLocationType(int2) == pT::Escaped);
    CHECK(biliardo.getLocationType(int3) == pT::BackHit);
  }

  // test punto di intersezione
  SUBCASE("Test punto di intersezione") {
    // particella 0
    CHECK(int0.x() == doctest::Approx(9.5238095));
    CHECK(int0.y() == doctest::Approx(9.5238095));
  }

  // calcolo angoli riflessi
  SUBCASE("Test riflessione") {
    float refl_0 = biliardo.reflect(p0);

    CHECK(refl_0 == doctest::Approx(5.39786));

    CHECK(biliardo.getHitDirection(p0.theta) ==
          particleSimulator::vecOrientation::DownRight);

    biliardo.reflect(p0);
  }
}