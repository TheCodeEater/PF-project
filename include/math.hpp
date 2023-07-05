#ifndef MATH_HPP
#define MATH_HPP

#include <type_traits>

#include "dataStructures.hpp"
namespace particleSimulator {
namespace constants {
// costanti per approx FP e troncamento
constexpr float eps() { return 1e-4; }
constexpr float trunc_prec() { return 100000; }
// offset che determina tolleranza nel determinare se un angolo è troppo vicino
// a pi/2
constexpr float angle_tolerance() { return 0.02f; }
}  // namespace constants

float arctan(float y, float x);  // arctan con valori tra 0 e 2pi

// funzione SQRT per exit point
exit_point getSQRT(exit_point const& value);

//funzione di radice quadrata con lo stesso nome -di quella di exit point
//così posso fare function overloading: se avessi chiamato getSQRT sqrt non avrei potuto usarla come overload
//differente di std::sqrt poiché sono in namespace diversi (particleSimulator e std): il compilatore vedrebbe solo una delle due

template <typename T>
T getSQRT(T const& value) {
  static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, exit_point>);
  return std::sqrt(value);
}

}  // namespace particleSimulator

#endif