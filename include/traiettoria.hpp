#ifndef TRAIETTORIA_HPP
#define TRAIETTORIA_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../Eigen/Dense"

const inline float pi=3.14159265358;

using Line = Eigen::ParametrizedLine<float, 2>;
namespace particleSimulator {
struct particle {
  Eigen::Vector2f pos{};  // pos sta per position
  double theta{};         // pos e theta da input (distribuzione)
};

class path {  // contiene i bordi del biliardo
  Line borderup_{};
  Line borderdown_{};

  //direzioni normali ai bordi. I vettori puntano verso l'interno della scatola del biliardo
  Eigen::Vector2f normal_up_{};
  Eigen::Vector2f normal_down_{};

 public:
  path(Line const&, Line const&);
  Eigen::Vector2f operator()(particle const&)
      const;  // operatore () per calcolare la traiettoria riflessa
};

}  // namespace particleSimulator

#endif