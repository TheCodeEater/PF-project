#include "../include/traiettoria.hpp"

#include <cmath>

namespace particleSimulator {
path::path(Line const& bup, Line const& bdown)
    : borderup_{bup}, borderdown_{bdown} {}

Eigen::Vector2f path::operator()(particle const& p) const {
  const Eigen::Vector2f dir{std::cos(p.theta),
                            std::sin(p.theta)};  // direzione particella
  // piccolo test
  assert(dir.norm() - 1 < 1e-5);
  const Eigen::ParametrizedLine<float, 2> trajectory{
      p.pos, dir};  // retta della direzione della particella

  const Eigen::Vector2f intersection = trajectory.intersectionPoint(
      borderup_);  // intersezione traiettoria con i bordi
  return intersection;
}

}  // namespace particleSimulator