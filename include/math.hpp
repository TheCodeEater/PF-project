#ifndef MATH_HPP
#define MATH_HPP

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
}  // namespace particleSimulator

#endif