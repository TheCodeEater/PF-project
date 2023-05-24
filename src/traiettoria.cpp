#include "../include/traiettoria.hpp"

#include <cmath>

namespace particleSimulator {
path::path(Line const& bup, Line const& bdown)
    : borderup_{bup}, borderdown_{bdown} {
        //TEST: correttezza della direzione dei vettori
        assert(borderup_.direction().x()>0 && borderup_.direction().y() <0); //test bordo sopra
        assert(borderdown_.direction().x()>0 && borderup_.direction.y() >0); //test bordo sotto
    }

Eigen::Vector2f path::operator()(particle const& p) const {
  const Eigen::Vector2f dir{std::cos(p.theta),
                            std::sin(p.theta)};  // direzione particella
  // piccolo test
  assert(dir.norm() - 1 < 1e-5);
  const Eigen::ParametrizedLine<float, 2> trajectory{
      p.pos, dir};  // retta della direzione della particella

  const Eigen::Vector2f intersection = trajectory.intersectionPoint(
      Eigen::Hyperplane{borderup_});  // intersezione traiettoria con i bordi
  return intersection;
}

Line path::getNormalUP(Eigen::Vector2f) const{//ottieni la retta normale al segmento superiore in un dato punto
    const double ang_coeff{}
    const Eigen::Vector2f intercept{0,}
}

}  // namespace particleSimulator