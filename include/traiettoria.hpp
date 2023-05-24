#ifndef TRAIETTORIA_HPP
#define TRAIETTORIA_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../Eigen/Dense"

using Line = Eigen::Hyperplane<float, 2>;
namespace particleSimulator {
struct particle {
  Eigen::Vector2f pos{};  // pos sta per position
  double theta{};         // pos e theta da input (distribuzione)
};

class path {  // contiene i bordi del biliardo
  Line borderup_{};
  Line borderdown_{};

 public:
  path(Line const&, Line const&);
  Eigen::Vector2f operator()(particle const&)
      const;  // operatore () per calcolare la traiettoria riflessa

  Line getNormal() const;
};
}  // namespace particleSimulator

#endif