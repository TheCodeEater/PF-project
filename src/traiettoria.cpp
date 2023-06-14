#include "../include/traiettoria.hpp"

#include <cmath>
#include <iostream>

namespace particleSimulator {
path::path(double r1, double r2, double l)
    : r1_{r1},
      r2_{r2},
      l_{l},
      borderup_{HLine::Through({0, r1_}, {l_, r2_})},
      borderdown_{HLine::Through({0, -r1_}, {l_, -r2_})},
      borderback_{HLine::Through({0,0},{0,1})},
      exit_line_{HLine::Through({l_,r2},{l,-r2})},
      horizontal_{1, 0} {
  // TEST: correttezza parametri
  assert(r1_ > 0);
  assert(r2_ > 0);
  assert(l_ > 0);

  Line up{borderup_};
  Line down{borderdown_};


  // Costruzione vettore normale - bordo sup
  const Eigen::Rotation2Df rot_sup{
      -pi / 2};  // rotazione di -pi/2 verso l'interno del biliardo
  normal_up_ = rot_sup * up.direction();

  // Vettore normale bordo giu
  const Eigen::Rotation2Df rot_inf{
      pi / 2};  // rotazione di -pi/2 verso l'interno del biliardo
  normal_down_ = rot_inf * down.direction();
}

intsect path::operator()(particle const& p) const {
  // CONDIZIONI INIZIALI
  assert(getLocationType(p.pos) == posTypes::Inside ||
         getLocationType(p.pos) ==
             posTypes::BackHit);  // devi essere dentro il biliardo
  assert(p.theta >= 0);           // angolo tra 0 e 2pi
  assert(p.theta <= 2 * pi);

  const Eigen::Vector2d dir{std::cos(p.theta),
                            std::sin(p.theta)};  // direzione particella
  // piccolo test
  assert(std::abs(dir.norm() - 1) <
         1e-3);  // test della norma del vettore uguale a 1
  const Eigen::ParametrizedLine<double, 2> trajectory{
      p.pos, dir};  // retta della direzione della particella

  // determina quale bordo puoi colpire
  // casi
  // bordo su, bordo giu
  // vettore orizzontale verso x negative, o verso x positive
  // calcola il bordo da colpire

  const intsect intersec = [this, &trajectory, &p] () -> intsect {//tipo esplicito
    vecOrientation orientation{getHitDirection(p.theta)};

    switch (orientation) {
      case vecOrientation::HorizontalLeft: {  // sbatti sempre sul bordo
        return {trajectory.intersectionPoint(borderback_),hitBorder::Back};
      }
      case vecOrientation::Right: {
        //verifica se esce
        const Eigen::Vector2d exit_intsec=exitIntersection(trajectory); //intersezione con la barra di uscita
        if(exit_intsec.y()<r2_ && exit_intsec.y()>-r2_){
          return {exit_intsec,hitBorder::Front};
        }else if(exit_intsec.y()>=r2_){
          return {trajectory.intersectionPoint(borderup_),hitBorder::Top};  // intersezione con sup
        }else if(exit_intsec.y()<=-r2_){
          return {trajectory.intersectionPoint(borderdown_),hitBorder::Bottom};  // intersezione con inf
        }else{
          throw std::logic_error("Impossibile determinare l'intersezione!");
        }
      }

      case vecOrientation::UpLeft:{ //in basso a sx: bordo dietro o basso
          const Eigen::Vector2d back_intsect{trajectory.intersectionPoint(borderback_)}; //intsect con la verticale
          //test intersezione verticale
          if(std::abs(std::abs(back_intsect.y())-r1_)<eps){ //colpo all'angolo: distanza da r1 entro i limiti di double
              return {back_intsect,hitBorder::Angle};
          }else if(std::abs(back_intsect.y())<=r1_-eps){//colpo al bordo dietro
              return {back_intsect,hitBorder::Back};
          }else if(std::abs(back_intsect.y())>=r1_+eps){//intersezione con il sup
              return {trajectory.intersectionPoint(borderup_), hitBorder::Top};  // intersezione con sup
          }
      }
      case vecOrientation::DownLeft:{ //in alto a sx: bordo dietro o alto
          const Eigen::Vector2d back_intsect{trajectory.intersectionPoint(borderback_)}; //intsect con la verticale
          //test intersezione verticale
          if(std::abs(std::abs(back_intsect.y())-r1_)<eps){ //colpo all'angolo
              return {back_intsect,hitBorder::Angle};
          }else if(std::abs(back_intsect.y())<=r1_-eps){//colpo al bordo dietro
              return {back_intsect,hitBorder::Back};
          }else if(std::abs(back_intsect.y())>=r1_+eps){//intersezione con inf
              return {trajectory.intersectionPoint(borderdown_),hitBorder::Bottom};  // intersezione con inf
          }
      }

        case vecOrientation::VerticalUp:{
              return {trajectory.intersectionPoint(borderup_), hitBorder::Top};  // intersezione con sup
        }
        case vecOrientation::VerticalDown:{
              return {trajectory.intersectionPoint(borderdown_),hitBorder::Bottom};  // intersezione con inf
        }
    }
  }();

  assert(getLocationType(intersec.point) != posTypes::Error);
  
  return intersec;
}

double path::reflect(particle& p) const {
  const intsect intsect = operator()(
      p);  // calcola il punto di collisione
  // determina se è il bordo su o il bordo giù, usando le coordinate del punto
  // di intersezione
  if(intsect.border==hitBorder::Front){ //particella uscita
    p.pos=intsect.point; //punto finale
    return p.theta;
  }else{
  Eigen::Vector2d const& normal_vect = [this,&intsect,&p]() -> vec{
    switch (intsect.border) { //esamina il bordo
      case hitBorder::Top:{
        return normal_up_;
      }
      case hitBorder::Bottom : {
        return normal_down_;
      }

      case hitBorder::Back:{
        return horizontal_;
      }
      case hitBorder::Angle:{
        //fai la parallela, orientata nella medesima direzione del vettore riflesso (angolo di incidenza 0)
        return {-std::cos(p.theta),-std::sin(p.theta)};
      }
      case hitBorder::Front:{
        throw std::logic_error("Non possibile, dovevi averlo gia gestito!");
      }
    }
  }();
  const Line normal{intsect.point, normal_vect};  // trova la normale
  // calcola l'angolo di incidenza
  // angolo della normale
  const double normal_angle = arctan(normal_vect.y(), normal_vect.x());
  // vettore traiettoria ribaltato
  const Eigen::Vector2d dir{-std::cos(p.theta), -std::sin(p.theta)};

  const double dir_angle =
      arctan(dir.y(), dir.x());  // calcola angolo del vettore

  assert(dir_angle <= 2 * pi);  // condizioni sulla convenzione degli angoli
  assert(dir_angle >= 0);

  const double phi_inc = normal_angle - dir_angle;  // angolo di incidenza

  // NOTA: phi non deve rispettare la condizione, il segno determina il verso
  // della rotazione
  // const double phi_inc=std::acos(normal.direction().dot(dir));
  // assert(std::abs(phi_inc)<=pi/2);
  // assert al momento non ripristinabile, fornisce problemi di geometria per
  // gli urti con il bordo posteriore risoluzione: prodotto scalare (vettoriale)
  // e con e sin

  const Eigen::Rotation2D rotation{2 *
                                   phi_inc};  // rotazione di 2*angolo incidenza

  const Eigen::Vector2d new_dir = rotation * dir;  // esegui la rotazione

  const double new_angle = arctan(new_dir.y(), new_dir.x());

  p.pos = intsect.point;
  p.theta = new_angle;

  return new_angle;
  }
}

double path::arctan(double y, double x) {
  const double theta = std::atan2f(y, x);
  // correggi l'angolo risultante in modo tale che sia compreso tra zero e 2pi,
  // non tra -pi e pi
  if (theta < 0) {
    return theta + 2 * pi;
  } else {
    return theta;
  }
}

posTypes path::getLocationType(Eigen::Vector2d const& v)
    const {  // determina il luogo del biliardo in cui si trova
  // assert; verifica che la x di v corrisponda al corrispondente valore Y
  // calcolato
  if (v.x() > 0 && v.x() < l_) {  // coordinata x entro i limiti del biliardo
    return posTypes::Inside;
  } else if (v.x() <= 0) {  // x negative: colpisci il fondo
    return posTypes::BackHit;
  } else if (v.x() >= l_) {  // fuori: fuggito
    return posTypes::Escaped;
  } else {  // altrimenti, problema
    return posTypes::Error;
  }
}
/*
vecOrientation path::getHitDirection(Eigen::Vector2d const& v) const{
    double angle=arctan(v.y(),v.x());
    return getHitDirection(angle);
}*/

vecOrientation path::getHitDirection(
    double angle) const {  // nota: accetta angoli tra -pi e +pi
  assert(angle >= 0 && angle <= 2 * pi);
  
  if(angle<=eps){ //angolo nullo: orizzontale destra
    return vecOrientation::Right;
  }else if(angle>eps && angle<pi/2-eps){ //alto a dx
    return vecOrientation::Right;
  }else if(std::abs(angle-pi/2)<=eps){
    return vecOrientation::VerticalUp;
  }else if(angle>pi/2+eps && angle<pi-eps){
    return vecOrientation::UpLeft;
  }else if(std::abs(angle-pi)<=eps){
    return vecOrientation::HorizontalLeft;
  }else if(angle>pi+eps && angle < 1.5f*pi-eps){
    return vecOrientation::DownLeft;
  }else if(std::abs(angle-1.5f*pi)<=eps){
    return vecOrientation::VerticalDown;
  }else if(angle>1.5f*pi+eps && angle < 2*pi-eps){
    return vecOrientation::Right;
  }else if(std::abs(angle-2*pi)<=eps){
    return vecOrientation::Right;
  }else{
    throw std::logic_error("Impossibile determinare la direzione del vettore!");
  }
}

Eigen::Vector2d path::exitIntersection(Line const& l) const{ //intersezione della retta data con quella di uscita
  return l.intersectionPoint(exit_line_);
}

double path::getR1() const{
  return r1_;
}
double path::getR2() const{
  return r2_;
}
double path::getL() const{
  return l_;
}

simulation::simulation(double r1, double r2, double l, int max_cycles)
    :  // costruttore
      simulator_{r1, r2, l},
      max_iterations_{max_cycles} {}

std::pair<std::vector<dottedLine>,exit_point> simulation::operator()(
    particle& p) const  // operatore di simulazione
{
  std::vector<dottedLine> trajs{};
  for (int i{0}; i < max_iterations_;
       ++i) {  // up to the maximum number of iterations
    // position vectors
    const Eigen::Vector2d old_pos{p.pos};

    simulator_.reflect(p);  // run the particle reflection

    Eigen::Vector2d const& curr_pos{
        p.pos};  // const reference to current position

    particleSimulator::dottedLine line{old_pos,
                                       curr_pos};  // create line trajectory
    trajs.push_back(line);                         // save into vector

    if (simulator_.getLocationType(p.pos) ==
        posTypes::Escaped) {  // se la particella esce, termina il ciclo
      break;
    }
  }

  //calcolo posizione finale
  if(simulator_.getLocationType(p.pos)==posTypes::Escaped){
    return std::make_pair(trajs,simulator_.getEscapePoint(trajs)); //restituisci la coppia di dati
  }else{
    return std::make_pair(trajs,exit_point{-10,-10}); //restituisci le traiettorie e un indicatore di failure
  }
}

exit_point path::getEscapePoint(vec const& p0, vec const& p1) const{
      //test che sia effettivamente fuggita
      assert(getLocationType(p1)==posTypes::Escaped);

      const Eigen::ParametrizedLine<double,2> line{Eigen::ParametrizedLine<double,2>::Through(p0,p1)};

      //calcolo Y: interseca con asse di uscita
      const Eigen::Vector2d escape_intersection{exitIntersection(line)}; //ottieni punto di fuga
      //test intersezo
      assert(escape_intersection.y()<=getR2()+1e-3);
      assert(escape_intersection.y()>=-getR2()-1e-3);

      const double escape_phi{std::atanf(line.direction().y()/line.direction().x())}; //angolo di uscita tra -pi/2 e pi/2

      return {escape_intersection.y(),escape_phi}; //restituisci il punto di fuga
}

  exit_point path::getEscapePoint(std::vector<dottedLine> const& trajectiories) const{
      //to do: aggiungere dei getter a path cosi da poter fare i test con gli assert
      //la funzione assume che la particella sia fuggita
      dottedLine const& last=trajectiories.back(); //ottieni traiettoria di
      const auto extremes=last.getExtremes();
      const Eigen::Vector2d p0=extremes.first;
      const Eigen::Vector2d p1=extremes.second;

      return getEscapePoint(p0,p1);

  }

}  // namespace particleSimulator