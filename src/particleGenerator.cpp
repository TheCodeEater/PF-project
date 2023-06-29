#include "../include/particleGenerator.hpp"

namespace particleSimulator {

randSimulator::randSimulator(randOptions options)
    :  // constructor
      simulator_{options.r1, options.r2, options.l},
      engine_{options.seed},
      pos_dist_{options.pos_mean, options.pos_sigma},
      angle_dist_{options.angle_mean, options.angle_sigma} {}

particle randSimulator::getParticle() {  // niente const, i generatori cambiano
                                         // lo stato interno
  // genera angolo - NOTA: angolo tra -bm::pi<float>()/2 bm::pi<float>()/2
  const float theta = angle_dist_(engine_);
  const float y = pos_dist_(engine_);
  // test sull'angolo

  if (std::abs(theta) >= bm::pi<float>() - angle_offset ||
      std::abs(y) > simulator_.getR1() - angle_offset) {  // se eccedi
    return getParticle();
  } else {
    particle p{{0, y},
               (theta < 0) ? 2 * bm::pi<float>() + theta
                           : theta};  // se l'angolo e' negativo, adatta la
                                      // convenzione sugli angoli

    p.theta = std::trunc(p.theta * path::trunc_prec) / path::trunc_prec;

    return p;
  }
}

std::normal_distribution<float> const& randSimulator::getPosGenerator() const {
  return pos_dist_;
}
std::normal_distribution<float> const& randSimulator::getAngleGenerator()
    const {
  return angle_dist_;
}

std::vector<exit_point> randSimulator::run(int n, int max_iterations) {
  std::vector<particle> particles{};

  std::generate_n(std::back_inserter(particles), n, [this]() {
    return getParticle();
  });  // esegui la generazione delle particelle

  // vettore dati di uscita
  std::vector<exit_point> exit_p{};
  // loop di calcolo
  std::for_each(
      particles.begin(), particles.end(),
      [this, &max_iterations, &exit_p](particle& p) {
        // esegui la riflessione
        for (int i{0}; i < max_iterations;
             ++i) {  // up to the maximum number of iterations
          // position vectors
          const particle old_p{p};

          simulator_.reflect(p);  // run the particle reflection

          if (simulator_.getLocationType(p.pos) == posTypes::Escaped ||
              simulator_.getLocationType(p.pos) ==
                  posTypes::BackHit) {  // se la particella esce, termina il
                                        // ciclo
            assert(simulator_.testOutConditions(p));
            exit_p.push_back(exit_point{p.pos.y(), p.theta});
            break;
          }
        }
      });

  return exit_p;
}

}  // namespace particleSimulator