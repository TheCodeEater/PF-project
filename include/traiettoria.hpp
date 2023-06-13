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

enum class vecOrientation { UpRight, UpLeft, DownLeft, DownRight, HorizontalLeft, HorizontalRight, VerticalUp, VerticalDown };

enum class hitBorder {Top, Bottom, Back, Front, Angle};
struct particle {
  Eigen::Vector2f pos{};  // pos sta per position
  double theta{};         // pos e theta da input (distribuzione)
};

struct exit_point{
  float y{};
  float theta{};
};

struct intsect{
  Eigen::Vector2f point{};
  hitBorder border{};
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
 using vec=Eigen::Vector2f;
  // path(Line const&, Line const&);
  path(float r1, float r2, float r3);
  intsect operator()(particle const&)
      const;  // operatore () per calcolare la traiettoria riflessa
  float reflect(particle&) const;
  posTypes getLocationType(Eigen::Vector2f const&) const;
  // vecOrientation getHitDirection(Eigen::Vector2f const&) const;
  vecOrientation getHitDirection(float angle) const;

  Eigen::Vector2f exitIntersection(Line const&) const;

  static float arctan(float y, float x);

  exit_point getEscapePoint(std::vector<dottedLine> const& trajectiories) const;
  exit_point getEscapePoint(vec const& p0, vec const& p1) const;

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
  
};

}  // namespace particleSimulator

#endif