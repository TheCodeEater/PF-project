#ifndef PARTICLE_GENERATOR_HPP
#define PARTICLE_GENERATOR_HPP

#include <random>

#include "io.hpp"
#include "traiettoria.hpp"
namespace particleSimulator {

class randSimulator {  // classe per la simulazione con i numeri casuali

  path simulator_;

  std::default_random_engine engine_;  // generatore

  std::normal_distribution<float> pos_dist_;  // pdf
  std::normal_distribution<float> angle_dist_;

  static constexpr float angle_offset{0.01f};

 public:
  randSimulator(randOptions options);  // costruttore

  particle getParticle();  // genera le particelle

  std::vector<exit_point> run(int n,
                              int max_iterations);  // esegui la simulazione

  std::normal_distribution<float> const& getPosGenerator() const;
  std::normal_distribution<float> const& getAngleGenerator() const;
};

}  // namespace particleSimulator

#endif