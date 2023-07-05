#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#if __INTELLISENSE__ //risolve un problema di vscode: gli oggetti di eigen non sono riconosciuti
#undef __ARM_NEON
#undef __ARM_NEON__
#endif

#include "../Eigen/Dense"
#include "../include/graphics.hpp"

namespace particleSimulator{
    //enum per: posizione particelle, orientazione, bordo colpito

enum class posTypes { Inside, Escaped, BackHit, Error };

enum class vecOrientation { Left, Right, VerticalUp, VerticalDown };

enum class hitBorder { Top, Bottom, Back, Front };
struct particle {
  Eigen::Vector2f pos{};  // vettore posizione
  float theta{};          // angolo traiettoria, tra 0 e 2pi, misurato dall'asse X in senso antiorario
};
struct exit_point { //punto di uscita, con angolo di traiettoria di uscita
  float y{};
  float theta{};

  exit_point& operator+=(exit_point const& rhs);
  exit_point& operator*=(exit_point const& rhs);
  exit_point& operator*=(int const& rhs);
  exit_point& operator/=(int const& rhs);

  exit_point operator+(exit_point const& rhs) const;
  exit_point operator*(exit_point const& rhs) const;
  exit_point operator/(int const& rhs) const;
  exit_point operator*(int const& rhs) const;
};

//funzione SQRT per exit point
exit_point getSQRT(exit_point const& value);

struct intsect { //intersezione
  Eigen::Vector2f point{};
  hitBorder border{};
};

struct results{
  std::vector<dottedLine> trajs{};
  exit_point ep{};
};
};

#endif