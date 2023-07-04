#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../include/statistics.hpp"

#include "doctest.h"

namespace st = particleSimulator::stats;  // alias namespace

TEST_CASE("Testing the class handling a floating point data sample") {
  st::Sample sample;

  SUBCASE("Removing an existing point") {  // test rimozione di punto esistente
    sample.add(1.5);
    sample.add(2.0);
    CHECK(sample.remove(2.0) == true);
    CHECK(sample.size() == 1);
  }

  SUBCASE(
      "Removing a non existing point") {  // test rimozione di punto inesistente
    sample.add(1.5);
    sample.add(2.0);
    CHECK(sample.remove(1.9) == false);
    CHECK(sample.size() == 2);
  }
  SUBCASE("Calling statistics() with two points") {  // test di statistics con
                                                     // campioni ridotti
    sample.add(1.0);
    sample.add(2.0);
    auto result = sample.statistics();
    CHECK(result.mean == doctest::Approx(1.5));
    CHECK(result.sigma == doctest::Approx(0.70710678));
    CHECK(result.mean_err == doctest::Approx(0.5));
  }
  SUBCASE("Testing sum operator") {  // test operatore di somma
    sample.add(1);
    st::Sample sample_two;
    sample_two.add(2);
    auto sum = sample + sample_two;
    CHECK(sum.size() == 2);
    if (sum.size() == 2) {
      CHECK(sum.entries()[0] == doctest::Approx(1.0));
      CHECK(sum.entries()[1] == doctest::Approx(2.0));
    }
  }
};

TEST_CASE("Big data test") {          // test con grandi dati
  std::ifstream in{"test/data.txt"};  // file di input

  assert(in.is_open());  // test che sia aperto

  st::Sample s{};
  while (in.good()) {  // lettura valori finché non si arriva a EOF
    float value{};
    in >> value;

    s.add(value);
  }

  st::Statistics stats = s.statistics();

  CHECK(stats.mean == doctest::Approx(3.00005));
  CHECK(stats.sigma == doctest::Approx(2.00171));
  CHECK(stats.kurtosis == doctest::Approx(0.00584769));
  CHECK(stats.skewness == doctest::Approx(-0.00401721));
};