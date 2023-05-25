#include "../include/traiettoria.hpp"

#include <cmath>

namespace particleSimulator {
path::path(float r1, float r2, float l)
    : r1_{r1}, r2_{r2}, l_{l},
    borderup_{Line::Through({0,r1_},{l_,r2_})}, borderdown_{Line::Through({0,-r1_},{l_,-r2_})} {
        //TEST: correttezza parametri
        assert(r1_>0);
        assert(r2_>0);
        assert(l_>0);
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
  //CONDIZIONI INIZIALI
  assert(p.pos.x()>=0); //devi essere dentro il biliardo

  const Eigen::Vector2f dir{std::cos(p.theta),
                            std::sin(p.theta)};  // direzione particella
  // piccolo test
  assert(dir.norm() - 1 < 1e-5);
  const Eigen::ParametrizedLine<float, 2> trajectory{
      p.pos, dir};  // retta della direzione della particella

//determina quale bordo puoi colpire
//casi
//bordo su, bordo giu
//vettore orizzontale verso x negative, o verso x positive

  const Eigen::Vector2f intersec_up = trajectory.intersectionPoint(
      Eigen::Hyperplane<float,2>{borderup_});  // intersezione traiettoria con il sup
  
  const Eigen::Vector2f intersec_down = trajectory.intersectionPoint(Eigen::Hyperplane<float,2>{borderdown_}); //intersezione con inf

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
    Eigen::Vector2f const& normal_vect{(intsect.y()>0)? normal_up_ : normal_down_};
    const Line normal{intsect,  normal_vect}; //trova la normale
    //calcola l'angolo di incidenza
    //angolo della normale
    float normal_angle=arctan(normal_vect.x(),normal_vect.y());
    //angolo traiettoria
    //vettore traiettoria
    Eigen::Vector2f dir{-std::cos(p.theta),
                            -std::sin(p.theta)};  // direzione particella, invertita per il calcolo della riflessione
    //angolo traiettoria
    float dir_angle=arctan(dir.x(),dir.y());

    float phi_inc=normal_angle-dir_angle; //angolo di incidenza

    //ruota il vettore direzione
    const Eigen::Rotation2Df reflection{phi_inc};
    dir=reflection*dir; //esegui la rotazione

    //aggiorna la particella
    p.pos=intsect;
    p.theta=arctan(dir.x(),dir.y());
}

float path::arctan(float x, float y){
    const float theta=std::atan2f(y,x);
    //correggi l'angolo risultante in modo tale che sia compreso tra zero e 2pi, non tra -pi e pi
    if(theta<0){
        return theta+2*pi;
    }else{
        return theta;
    }
}

bool path::isInside(Eigen::Vector2f const& v) const{
    if(v.x()>0 && v.x()<=)
}

/*
bool path::toUp(Eigen::Vector2f const& v){
    float angle=std::atan2f(v.y(),v.x());
    
    if(angle>0){
        return true;
    }else{
        return false;
    }
}

bool path::toDown(Eigen::Vector2f const& v){
    float angle=std::atan2f(v.y(),v.x());
    
    if(angle>0){
        return false;
    }else{
        return true;
    }
}

bool path::toHoriz(Eigen::Vector2f const& v){
    float angle=std::atan2f(v.y(),v.x());
    if(std::abs(angle)<1e-3){
        return true;
    }else{
        return false;
    }
}
*/
}  // namespace particleSimulator