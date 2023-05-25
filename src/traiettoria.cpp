#include "../include/traiettoria.hpp"

#include <cmath>

namespace particleSimulator {
path::path(Line const& bup, Line const& bdown)
    : borderup_{bup}, borderdown_{bdown} {
        //TEST: correttezza della direzione dei vettori
        assert(borderup_.direction().x()>0); assert(borderup_.direction().y() <0); //test bordo sopra
        assert(borderdown_.direction().x()>0); assert(borderdown_.direction().y() >0); //test bordo sotto

        //Costruzione vettore normale - bordo sup
        const Eigen::Rotation2Df rot_sup{-pi/2}; //rotazione di -pi/2 verso l'interno del biliardo
        normal_up_=rot_sup*borderup_.direction();

        //Vettore normale bordo giu
        const Eigen::Rotation2Df rot_inf{pi/2}; //rotazione di -pi/2 verso l'interno del biliardo
        normal_down_=rot_inf*borderdown_.direction();
    }

Eigen::Vector2f path::operator()(particle const& p) const {
  const Eigen::Vector2f dir{std::cos(p.theta),
                            std::sin(p.theta)};  // direzione particella
  // piccolo test
  assert(dir.norm() - 1 < 1e-5);
  const Eigen::ParametrizedLine<float, 2> trajectory{
      p.pos, dir};  // retta della direzione della particella

  const Eigen::Vector2f intersection = trajectory.intersectionPoint(
      Eigen::Hyperplane<float,2>{borderup_});  // intersezione traiettoria con i bordi

  //TEST INTERSEZIONE
  if(intersection.x()<=0){//se l'intersezione è in x negative (ossia se collidiamo con il bordo laterale)
    return trajectory.intersectionPoint(Eigen::Hyperplane<float,2>::Through({0,0},{0,1}));
  }else{
    return intersection;
  }
}

}  // namespace particleSimulator