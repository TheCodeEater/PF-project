#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#if __INTELLISENSE__ //risolve un problema di vscode: gli oggetti di eigen non sono riconosciuti
#undef __ARM_NEON
#undef __ARM_NEON__
#endif

#include <SFML/Graphics.hpp>
#include "boost/math/constants/constants.hpp"

#include "../Eigen/Dense"
#include "../include/graphics.hpp"

namespace particleSimulator {

namespace constants{
    //costanti per approx FP e troncamento  
  constexpr float eps(){
    return 1e-4;
  }
  constexpr float trunc_prec(){
    return 100000;
  }
  // offset che determina tolleranza nel determinare se un angolo è troppo vicino a pi/2
  constexpr float angle_tolerance(){ 
    return 0.02f;
  }
}

//alis per i tipi di eigen e namespace boost
using Line = Eigen::ParametrizedLine<float, 2>;
using HLine = Eigen::Hyperplane<float, 2>;
namespace bm = boost::math::constants;

float arctan(float y, float x); //arctan con valori tra 0 e 2pi

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

  exit_point operator+(exit_point const& rhs);
  exit_point operator*(exit_point const& rhs);
};

//funzione SQRT per exit point
exit_point sqrt(exit_point const& value);

struct intsect { //intersezione
  Eigen::Vector2f point{};
  hitBorder border{};
};

struct results{
  std::vector<dottedLine> trajs{};
  exit_point ep{};
};

class path { //oggetto di calcolo delle traiettorie
  // parametri biliardo
  float r1_{};
  float r2_{};
  float l_{};

  //bordi
  HLine borderup_{};
  HLine borderdown_{};

  //pseudobordi (usati solo per calcolare intersezioni)
  HLine borderback_{};
  HLine borderfront_{};

  // direzioni normali ai bordi. I vettori puntano verso l'interno della scatola
  // del biliardo
  Eigen::Vector2f normal_up_{};
  Eigen::Vector2f normal_down_{};

  // funzione di rotazione del vettore (per eseguire la riflessione)
  void rotate(particle&, Eigen::Vector2f const&, intsect const&) const;

 public:
  //using vec = Eigen::Vector2f;
  // path(Line const&, Line const&);
  path(float r1, float r2, float r3);
  intsect operator()(particle const&)
      const;  // operatore () per calcolare la traiettoria riflessa

  void reflect(particle&) const; //esegui la riflessione
  
  posTypes getLocationType(Eigen::Vector2f const&) const; //determina il punto dove ti trovi
  vecOrientation getHitDirection(float const& angle) const; //determina direzione generale del vettore

  // getter
  float getR1() const;
  float getR2() const;
  float getL() const;

  // test delle condizioni di uscita (verifica che siano rispettate)
  bool testOutConditions(particle const&) const;
};

class simulation {  // classe che gestisce la simulazione
  path simulator_; //oggetto path per eseguire la simulazione
  int max_iterations_{};

 public:
  simulation(float r1, float r2, float l, int max_cycles);  // constructor

  results operator()(
      particle& p) const;  // operatore di simulazione

  std::vector<particle> getSequence(particle& p, int max_iterations) const; //simulazione salvando la sequenza delle posizioni occupate
};

}  // namespace particleSimulator

#endif