#include "../include/dataStructures.hpp"

namespace particleSimulator {

// operatori aritmetici @=
exit_point& exit_point::operator+=(exit_point const& rhs) {
  y += rhs.y;
  theta += rhs.theta;

  return *this;
}
exit_point& exit_point::operator-=(exit_point const& rhs) {
  y -= rhs.y;
  theta -= rhs.theta;

  return *this;
}
exit_point& exit_point::operator*=(exit_point const& rhs) {
  y *= rhs.y;
  theta *= rhs.theta;

  return *this;
}
exit_point& exit_point::operator/=(exit_point const& rhs) {
  y /= rhs.y;
  theta /= rhs.theta;

  return *this;
}

// operatori arimetici @
exit_point operator+(exit_point const& lhs, exit_point const& rhs) {
  exit_point result{lhs};
  result += rhs;
  return result;
}
exit_point operator-(exit_point const& lhs, exit_point const& rhs) {
  exit_point result{lhs};
  result -= rhs;
  return result;
}
exit_point operator*(exit_point const& lhs, exit_point const& rhs) {
  exit_point result{lhs};
  result *= rhs;
  return result;
}
exit_point operator/(exit_point const& lhs, exit_point const& rhs) {
  exit_point result{lhs};
  result /= rhs;
  return result;
}

// la funzione tratta exit_point come una struct accumulatrice, eseguendo la
// radice di entrambi i valori
exit_point getSQRT(exit_point const& value) {
  return {std::sqrt(value.y),
          std::sqrt(value.theta)};  // esegui la radice su ogni membro
}
}  // namespace particleSimulator