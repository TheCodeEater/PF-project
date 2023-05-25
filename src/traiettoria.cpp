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

void path::reflect(particle& p) const{
    const Eigen::Vector2f intsect=operator()(p); //calcola il punto di collisione
    //determina se è il bordo su o il bordo giù, usando le coordinate del punto di intersezione
    const Line& normal_vect=(intsect.y()>0)? normal_up_ : normal_down_;
    const Line normal{intsect,  normal_vect}; //trova la normale
    //calcola l'angolo di incidenza
    //angolo della normale
    float normal_angle=arctan(normal_vect.x(),normal_vect.y());
    //angolo traiettoria
    //vettore traiettoria
    const Eigen::Vector2f dir{std::cos(p.theta),
                            std::sin(p.theta)};  // direzione particella
    //prima flippa il vettore
    dir*=-1;
    //angolo traiettoria
    float dir_angle=arctan(dir.x(),dir.y());

    float phi_inc=normal_angle-dir_angle; //angolo di incidenza

    //ruota il vettore direzione
    Eigen::Rotation2Df reflection{phi_inc};
    dir*=reflection; //esegui la rotazione

    //aggiorna la particella
    p.pos=intsect;
    p.angle=arctan(dir.x(),dir.y());
}

float path::arctan(float x, float y){
    const float theta=std::atan2f(x,y);
    //correggi l'angolo risultante in modo tale che sia compreso tra zero e 2pi, non tra -pi e pi
    if(theta<0){
        return theta+2*pi;
    }else{
        return theta;
    }
}

}  // namespace particleSimulator