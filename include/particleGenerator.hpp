#ifndef PARTICLE_GENERATOR_HPP
#define PARTICLE_GENERATOR_HPP

#include <random>

#include "io.hpp"
#include "trajectory.hpp"
namespace particleSimulator {

class randSimulator {  // classe per la simulazione con i numeri casuali

  path simulator_;  // oggetto di simulazione

  std::default_random_engine engine_;  // generatore di numeri casuali

  std::normal_distribution<float> pos_dist_;  // pdf gaussiane
  std::normal_distribution<float> angle_dist_;

  static constexpr float angle_offset{
      0.02f};  // offset che determina tolleranza nel determinare se un angolo è
               // troppo vicino a pi/2

 public:
  randSimulator(randOptions options);  // costruttore

  particle getParticle();  // genera le particelle

  std::vector<exit_point> run(int n,
                              int max_iterations);  // esegui la simulazione

  // getter
  std::normal_distribution<float> const& getPosGenerator() const;
  std::normal_distribution<float> const& getAngleGenerator() const;
};

}  // namespace particleSimulator

#endif