#include "../include/dataStructures.hpp"

namespace particleSimulator{
    //overload operatori exit_point e sqrt

exit_point& exit_point::operator+=(exit_point const& rhs){
  y+=rhs.y;
  theta+=rhs.theta;

  return *this;
}
exit_point& exit_point::operator*=(exit_point const& rhs){
  y*=rhs.y;
  theta*=rhs.theta;

  return *this;
}

 exit_point& exit_point::operator/=(int const& rhs){
  y/=rhs;
  theta/=rhs;
 }

exit_point exit_point::operator+(exit_point const& rhs) const{
  exit_point lhs{*this};
  lhs+=rhs;
  return lhs;
}
exit_point exit_point::operator*(exit_point const& rhs) const{
  exit_point lhs{*this};
  lhs*=rhs;
  return lhs;
}

exit_point exit_point::operator/(int const& rhs) const{
  exit_point ep{*this};
  ep/=rhs;
  return ep;
 }

//la funzione tratta exit_point come una struct accumulatrice, eseguendo la radice di entrambi i valori
exit_point getSQRT(exit_point const& value){
  return {std::sqrt(value.y),std::sqrt(value.theta)}; //esegui la radice su ogni membro
}
}