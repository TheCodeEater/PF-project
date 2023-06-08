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

#include <type_traits>

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

class simulation{ //classe che gestisce la simulazione
  path simulator_;
  int max_iterations_{};

  public:
    simulation(float r1, float r2, float l, int max_cycles); //construcotr

    std::vector<dottedLine> operator()(particle& p) const//operatore di simulazione
    {
      std::vector<dottedLine> trajs{};
      for(int i{0}; i<max_iterations_; ++i){//up to the maximum number of iterations
        //position vectors
        const Eigen::Vector2f old_pos{p.pos};

        simulator_.reflect(p); //run the particle reflection

        Eigen::Vector2f const& curr_pos{p.pos}; //const reference to current position

        particleSimulator::dottedLine line{{old_pos.x(),old_pos.y()},{curr_pos.x(),curr_pos.y()}}; //create line trajectory
        trajs.push_back(line); //save into vector

        if(simulator_.getLocationType(p.pos)==posTypes::Escaped){ //se la particella esce, termina il ciclo
          break;
        }
      }

      return trajs;
    }

};

}  // namespace particleSimulator

#endif