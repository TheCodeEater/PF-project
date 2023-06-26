#ifndef TRAIETTORIA_HPP
#define TRAIETTORIA_HPP

#if __INTELLISENSE__
#undef __ARM_NEON
#undef __ARM_NEON__
#endif

#include <SFML/Graphics.hpp>
#include <boost/math/constants/constants.hpp>
#include <cmath>

#include "../Eigen/Dense"
#include "../include/graphics.hpp"

namespace particleSimulator {

using Line = Eigen::ParametrizedLine<float, 2>;
using HLine = Eigen::Hyperplane<float, 2>;
namespace bm = boost::math::constants;

float arctan(float y, float x);

enum class posTypes { Inside, Escaped, BackHit, Error };

enum class vecOrientation {
  Left,
  Right,
  VerticalUp,
  VerticalDown
};

enum class hitBorder { Top, Bottom, Back, Front, Angle };
struct particle {
  Eigen::Vector2f pos{};  // pos sta per position
  float theta{};         // pos e theta da input (distribuzione)
};

struct exit_point {
  float y{};
  float theta{};
};

struct intsect {
  Eigen::Vector2f point{};
  hitBorder border{};
};

class path {  // contiene i bordi del biliardo
  // parametri biliardo
  float r1_{};
  float r2_{};
  float l_{};

  HLine borderup_{};
  HLine borderdown_{};
  HLine borderback_{};

  // linea di uscita
  HLine exit_line_{};

  // direzioni normali ai bordi. I vettori puntano verso l'interno della scatola
  // del biliardo
  Eigen::Vector2f normal_up_{};
  Eigen::Vector2f normal_down_{};
  Eigen::Vector2f horizontal_{};

 public:
  using vec = Eigen::Vector2f;
  // path(Line const&, Line const&);
  path(float r1, float r2, float r3);
  intsect operator()(particle const&)
      const;  // operatore () per calcolare la traiettoria riflessa
  void reflect(particle&) const;
  posTypes getLocationType(Eigen::Vector2f const&) const;
  // vecOrientation getHitDirection(Eigen::Vector2f const&) const;
  vecOrientation getHitDirection(float const& angle) const;

  Eigen::Vector2f exitIntersection(Line const&) const;

  static constexpr float eps{1e-4};
  static constexpr int trunc_prec{10000};

  //exit_point getEscapePoint(std::vector<dottedLine> const& trajectiories) const;
  //exit_point getEscapePoint(vec const& p0, vec const& p1) const;

  // getter
  float getR1() const;
  float getR2() const;
  float getL() const;
};

class simulation {  // classe che gestisce la simulazione
  path simulator_;
  int max_iterations_{};

 public:
  simulation(float r1, float r2, float l, int max_cycles);  // construcotr

  std::pair<std::vector<dottedLine>, exit_point> operator()(
      particle& p) const;  // operatore di simulazione

  std::vector<particle> getSequence(particle& p, int max_iterations) const;
};

}  // namespace particleSimulator

#endif