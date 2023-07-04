#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <fstream>

#include "../include/particleGenerator.hpp"
#include "../include/traiettoria.hpp"
#include "doctest.h"

TEST_CASE("Test della generazione in range dei numeri") {
  const float r1{400};  // parametro r1
  const float r2{200};
  const float l{700};
  const unsigned seed{std::random_device{}()};

  std::cout << "Seed: " << seed << "\n";

  particleSimulator::path biliardo{r1, r2, l};

  particleSimulator::randSimulator rs{particleSimulator::randOptions{r1, r2, l, 0.f, 80.f, 0.f,
                                       particleSimulator::bm::pi<float>() / 8,
                                       seed,false}};

  std::vector<particleSimulator::exit_point> exit{};

  try {
    exit = rs.run(1000000, 200);
  } catch (std::exception const& e) {
    std::cout << e.what() << "\n";
    throw;
  }
}