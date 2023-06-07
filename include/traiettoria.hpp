#ifndef TRAIETTORIA_HPP
#define TRAIETTORIA_HPP

#if __INTELLISENSE__
#undef __ARM_NEON
#undef __ARM_NEON__
#endif

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../Eigen/Dense"

static const inline float pi=2*std::atan(INFINITY);

using Line = Eigen::ParametrizedLine<float, 2>;
namespace particleSimulator {

enum class posTypes{
  Inside,
  Escaped,
  BackHit,
  Error
};

enum class vecOrientation{
  Up,
  Down,
  HorizontalLeft,
  HorizontalRight
};
struct particle {
  Eigen::Vector2f pos{};  // pos sta per position
  double theta{};         // pos e theta da input (distribuzione)
};

class path {  // contiene i bordi del biliardo
  //parametri biliardo
  float r1_{};
  float r2_{};
  float l_{};

  Line borderup_{};
  Line borderdown_{};

  //direzioni normali ai bordi. I vettori puntano verso l'interno della scatola del biliardo
  Eigen::Vector2f normal_up_{};
  Eigen::Vector2f normal_down_{};
  Eigen::Vector2f horizontal_{};

 public:
  //path(Line const&, Line const&);
  path(float r1, float r2, float r3);
  Eigen::Vector2f operator()(particle const&)
      const;  // operatore () per calcolare la traiettoria riflessa
  float reflect(particle& ) const;
  posTypes getLocationType(Eigen::Vector2f const&) const;
  //vecOrientation getHitDirection(Eigen::Vector2f const&) const;
  vecOrientation getHitDirection(float angle) const;

  static float arctan(float y, float x);
};

}  // namespace particleSimulator

#endif