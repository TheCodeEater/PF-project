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

static const inline double pi = 2 * std::atan(INFINITY);

namespace particleSimulator {

using Line = Eigen::ParametrizedLine<double, 2>;
using HLine = Eigen::Hyperplane<double,2>;

enum class posTypes { Inside, Escaped, BackHit, Error };

enum class vecOrientation {  UpLeft, DownLeft, Right, HorizontalLeft, VerticalUp, VerticalDown };

enum class hitBorder {Top, Bottom, Back, Front, Angle};
struct particle {
  Eigen::Vector2d pos{};  // pos sta per position
  double theta{};         // pos e theta da input (distribuzione)
};

struct exit_point{
  double y{};
  double theta{};
};

struct intsect{
  Eigen::Vector2d point{};
  hitBorder border{};
};

class path {  // contiene i bordi del biliardo
  // parametri biliardo
  double r1_{};
  double r2_{};
  double l_{};

  HLine borderup_{};
  HLine borderdown_{};
  HLine borderback_{};

  //linea di uscita
  HLine exit_line_{};

  // direzioni normali ai bordi. I vettori puntano verso l'interno della scatola
  // del biliardo
  Eigen::Vector2d normal_up_{};
  Eigen::Vector2d normal_down_{};
  Eigen::Vector2d horizontal_{};

 public:
 using vec=Eigen::Vector2d;
  // path(Line const&, Line const&);
  path(double r1, double r2, double r3);
  intsect operator()(particle const&)
      const;  // operatore () per calcolare la traiettoria riflessa
  double reflect(particle&) const;
  posTypes getLocationType(Eigen::Vector2d const&) const;
  // vecOrientation getHitDirection(Eigen::Vector2d const&) const;
  vecOrientation getHitDirection(double angle) const;

  Eigen::Vector2d exitIntersection(Line const&) const;

  static double arctan(double y, double x);
  static constexpr double eps{1e-3};

  exit_point getEscapePoint(std::vector<dottedLine> const& trajectiories) const;
  exit_point getEscapePoint(vec const& p0, vec const& p1) const;

  //getter
  double getR1() const;
  double getR2() const;
  double getL() const;
};

class simulation {  // classe che gestisce la simulazione
  path simulator_;
  int max_iterations_{};

 public:
  simulation(double r1, double r2, double l, int max_cycles);  // construcotr

  std::pair<std::vector<dottedLine>,exit_point> operator()(
      particle& p) const;  // operatore di simulazione
  
};

}  // namespace particleSimulator

#endif