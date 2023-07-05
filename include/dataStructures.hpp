#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#if __INTELLISENSE__ //risolve un problema di vscode: gli oggetti di eigen non sono riconosciuti
#undef __ARM_NEON
#undef __ARM_NEON__
#endif

#include <type_traits>

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

  //operatori @= con oggetti dello stesso tipo
  exit_point& operator+=(exit_point const& rhs);
  exit_point& operator-=(exit_point const& rhs);
  exit_point& operator*=(exit_point const& rhs);
  exit_point& operator/=(exit_point const& rhs);

  //operatori @= con altri numeri
    template<typename T>
  exit_point operator-=(T const& rhs){
    static_assert(std::is_arithmetic_v<T>); //verifica che sia un valore numerico
    y-=rhs;
    theta-=rhs;

    return *this;
  }
  template<typename T>
  exit_point operator*=(T const& rhs){
    static_assert(std::is_arithmetic_v<T>); //verifica che sia un valore numerico
    y*=rhs;
    theta*=rhs;

    return *this;
  }
  template<typename T>
  exit_point operator/=(T const& rhs){
    static_assert(std::is_arithmetic_v<T>); //verifica che sia un valore numerico
    y/=rhs;
    theta/=rhs;
    return *this;
  }
};

//operatori di exit_point operator@ (senza =)
exit_point operator+(exit_point const& lhs,exit_point const& rhs);
exit_point operator-(exit_point const& lhs,exit_point const& rhs);
exit_point operator*(exit_point const& lhs,exit_point const& rhs);
exit_point operator/(exit_point const& lhs,exit_point const& rhs);

//operatori di exit_point aritmetici operator@ con altri numeri
  template<typename T>
  exit_point operator*(exit_point const& lhs,T const& rhs){
    static_assert(std::is_arithmetic_v<T>); //verifica che sia un valore numerico
    
    exit_point result{lhs};
    result*=rhs;

    return result;
  }
  template<typename T>
  exit_point operator/(exit_point const& lhs,T const& rhs){
    static_assert(std::is_arithmetic_v<T>); //verifica che sia un valore numerico
    
    exit_point result{lhs};
    result/=rhs;

    return result;
  }

    template<typename T>
  exit_point operator-(exit_point const& lhs,T const& rhs){
    static_assert(std::is_arithmetic_v<T>); //verifica che sia un valore numerico
    exit_point result{lhs};
    result-=rhs;

    return result;
  }

    template<typename T>
  exit_point operator*(T const& lhs,exit_point const& rhs){
    return rhs*lhs;
  }
  template<typename T>
  exit_point operator/(T const& lhs,exit_point const& rhs){
    return rhs/lhs;
  }

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