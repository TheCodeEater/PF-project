#include "../include/traiettoria.hpp"

#include <cmath>
#include <iostream>

namespace particleSimulator {
path::path(float r1, float r2, float l)
    : r1_{r1}, r2_{r2}, l_{l},
    borderup_{Line::Through({0,r1_},{l_,r2_})}, borderdown_{Line::Through({0,-r1_},{l_,-r2_})}, horizontal_{1,0} {
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
  assert(getLocationType(p.pos)==posTypes::Inside || getLocationType(p.pos)==posTypes::BackHit); //devi essere dentro il biliardo
  assert(p.theta>=0); //angolo tra 0 e 2pi
  assert(p.theta<=2*pi);

  const Eigen::Vector2f dir{std::cos(p.theta),
                            std::sin(p.theta)};  // direzione particella
  // piccolo test
  assert(std::abs(dir.norm() - 1) < 1e-3); //test della norma del vettore uguale a 1
  const Eigen::ParametrizedLine<float, 2> trajectory{
      p.pos, dir};  // retta della direzione della particella

//determina quale bordo puoi colpire
//casi
//bordo su, bordo giu
//vettore orizzontale verso x negative, o verso x positive
 //calcola il bordo da colpire

const Eigen::Vector2f intersec=[this,&trajectory,&p](){
    vecOrientation orientation{getHitDirection(p.theta)};

    switch(orientation){
    case vecOrientation::HorizontalLeft :{//sbatti sempre sul bordo
        return trajectory.intersectionPoint(
            Eigen::Hyperplane<float,2>{Line::Through({0,0},{0,1})});
    }

    case vecOrientation::HorizontalRight :{//puoi sbattere sopra o sotto
        //e' importante selezionare il bordo corretto poiche l'intersezione con entrambi si trova, ma una e' fuori
        //mentre l'altra no
        //idea: le fai entrambi e restituisci quella che sta dentro, se c'è
        const Eigen::Vector2f int_sup=trajectory.intersectionPoint(
            Eigen::Hyperplane<float,2>{borderup_}); //intersezione con sup
        
        const Eigen::Vector2f int_inf= trajectory.intersectionPoint(
            Eigen::Hyperplane<float,2>{borderdown_}); //intersezione con inf

        //
        const posTypes pos_sup=getLocationType(int_sup);
        const posTypes pos_inf=getLocationType(int_inf);

        //test della correttezza
        assert(pos_sup!=posTypes::Error);
        assert(pos_inf!=posTypes::Error);

        assert(pos_sup!=posTypes::BackHit);
        assert(pos_inf!=posTypes::BackHit);

        if(pos_sup==posTypes::Inside){//l'intersezione con il sup appartiene
            return int_sup;
        }else if(pos_inf==posTypes::Inside){ //con l'inf
            return int_inf;
        }else{
            return int_sup;//scelta a caso, inf sarebbe equivalente
        }

    }

    case vecOrientation::Up : {
        return trajectory.intersectionPoint(
            Eigen::Hyperplane<float,2>{borderup_}); //intersezione con sup
    }

    case vecOrientation::Down : {
        return trajectory.intersectionPoint(
            Eigen::Hyperplane<float,2>{borderdown_}); //intersezione con inf
    }
}
}();

  
  assert(getLocationType(intersec)!=posTypes::Error);
  //TEST INTERSEZIONE
  if(getLocationType(intersec)==posTypes::BackHit){//se l'intersezione è in x negative (ossia se collidiamo con il bordo laterale)
    return trajectory.intersectionPoint(Eigen::Hyperplane<float,2>::Through({0,0},{0,1}));
  }else{
    return intersec;
  }
  //ATTENZIONE: rimuovere questo pezzo ed inserirlo in una funzione ad hoc, che richiama internamente l'operatore
}

float path::reflect(particle& p) const{
    const Eigen::Vector2f intsect=operator()(p); //calcola il punto di collisione
    //determina se è il bordo su o il bordo giù, usando le coordinate del punto di intersezione
    Eigen::Vector2f const& normal_vect=[this,&p,&intsect](){

        switch(getHitDirection(p.theta)){
            case vecOrientation::Up: {
                return (intsect.y()>0) ? normal_up_ : horizontal_; //se vai verso l'alto, verifica se sbatti prima
            }

            case vecOrientation::Down:{
                return (intsect.y()>0) ? horizontal_ : normal_down_;
            }

            case vecOrientation::HorizontalLeft: {
                return horizontal_;
            }

            case vecOrientation::HorizontalRight: {
                return (intsect.y()>0) ? normal_up_ : normal_down_;
            }

        }
    }();
    const Line normal{intsect,  normal_vect}; //trova la normale
    //calcola l'angolo di incidenza
    //angolo della normale
    const float normal_angle=arctan(normal_vect.y(),normal_vect.x());
    //vettore traiettoria ribaltato
    Eigen::Vector2f dir{-std::cos(p.theta),-std::sin(p.theta)};

    const float dir_angle=arctan(dir.y(),dir.x()); //calcola angolo del vettore

    assert(dir_angle<=2*pi); //condizioni sulla convenzione degli angoli
    assert(dir_angle>=0);

    const float phi_inc=normal_angle-dir_angle; //angolo di incidenza
    std::cout<<"Angolo normale: "<<normal_angle<<"\n";
    std::cout<<"Angolo direzionale: "<<dir_angle<<"\n";
     //NOTA: phi non deve rispettare la condizione, il segno determina il verso della rotazione
    //const float phi_inc=std::acos(normal.direction().dot(dir));
    //assert(std::abs(phi_inc)<=pi/2);

    const Eigen::Rotation2D rotation{2*phi_inc};//rotazione di 2*angolo incidenza

    dir=rotation*dir; //esegui la rotazione

    const float new_angle=arctan(dir.y(),dir.x());

    p.pos=intsect;
    p.theta=new_angle;

    return new_angle;
}

float path::arctan(float y, float x){
    const float theta=std::atan2f(y,x);
    //correggi l'angolo risultante in modo tale che sia compreso tra zero e 2pi, non tra -pi e pi
    if(theta<0){
        return theta+2*pi;
    }else{
        return theta;
    }
}

posTypes path::getLocationType(Eigen::Vector2f const& v) const{ //determina il luogo del biliardo in cui si trova
    //assert; verifica che la x di v corrisponda al corrispondente valore Y calcolato
    if(v.x()>0 && v.x()<=l_){//coordinata x entro i limiti del biliardo
        return posTypes::Inside;
    }else if(v.x()<=0){//x negative: colpisci il fondo
        return posTypes::BackHit;
    }else if(v.x()>=l_){//fuori: fuggito
        return posTypes::Escaped;
    }else{//altrimenti, problema
        return posTypes::Error;
    }
}
/*
vecOrientation path::getHitDirection(Eigen::Vector2f const& v) const{
    float angle=arctan(v.y(),v.x());
    return getHitDirection(angle);
}*/

vecOrientation path::getHitDirection(float angle) const{//nota: accetta angoli tra -pi e +pi
    assert(angle>=0 && angle <=2*pi);
    if(std::abs(angle)<1e-3){//angolo nullo (con aritmetica floating point no ==0)
        return vecOrientation::HorizontalRight;
    }else if(std::abs(angle-pi)<1e-3){
        return vecOrientation::HorizontalLeft;
    }else if(angle>=pi && angle<=2*pi){//negativo, verso il basso
        return vecOrientation::Down;
    }else if(angle>=0 && angle<=pi){//positivo, vai verso l'alto
        return vecOrientation::Up;
    }else{
        throw std::logic_error("Impossibile determinare l'orientazione del vettore!");
    }
}

}  // namespace particleSimulator