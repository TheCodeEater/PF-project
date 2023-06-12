#ifndef TRAIETTORIA_HPP
#define TRAIETTORIA_HPP

#if __INTELLISENSE__
#undef __ARM_NEON
#undef __ARM_NEON__
#endif

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../Eigen/Dense"
#include "../include/graphics.hpp"

static const inline float pi = 2 * std::atan(INFINITY);

using Line = Eigen::ParametrizedLine<float, 2>;
namespace particleSimulator {

enum class posTypes { Inside, Escaped, BackHit, Error };

enum class vecOrientation { Up, Down, HorizontalLeft, HorizontalRight };
struct particle {
  Eigen::Vector2f pos{};  // pos sta per position
  double theta{};         // pos e theta da input (distribuzione)
};

struct exit_point{
  float y{};
  float theta{};
};

class path {  // contiene i bordi del biliardo
  // parametri biliardo
  float r1_{};
  float r2_{};
  float l_{};

  Line borderup_{};
  Line borderdown_{};

  //linea di uscita
  Eigen::Hyperplane<float,2> exit_line_{};

  // direzioni normali ai bordi. I vettori puntano verso l'interno della scatola
  // del biliardo
  Eigen::Vector2f normal_up_{};
  Eigen::Vector2f normal_down_{};
  Eigen::Vector2f horizontal_{};

 public:
  // path(Line const&, Line const&);
  path(float r1, float r2, float r3);
  Eigen::Vector2f operator()(particle const&)
      const;  // operatore () per calcolare la traiettoria riflessa
  float reflect(particle&) const;
  posTypes getLocationType(Eigen::Vector2f const&) const;
  // vecOrientation getHitDirection(Eigen::Vector2f const&) const;
  vecOrientation getHitDirection(float angle) const;

  Eigen::Vector2f exitIntersection(Line const&) const;

  static float arctan(float y, float x);

  //getter
  float getR1() const;
  float getR2() const;
  float getL() const;
};

class simulation {  // classe che gestisce la simulazione
  path simulator_;
  int max_iterations_{};

 public:
  simulation(float r1, float r2, float l, int max_cycles);  // construcotr

  std::pair<std::vector<dottedLine>,exit_point> operator()(
      particle& p) const;  // operatore di simulazione
  
  exit_point getEscapePoint(std::vector<dottedLine> const& trajectiories) const;
};

}  // namespace particleSimulator

#endif