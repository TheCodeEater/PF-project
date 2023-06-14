#include "../include/traiettoria.hpp"

#include <cmath>
#include <iostream>

namespace particleSimulator {
path::path(float r1, float r2, float l)
    : r1_{r1},
      r2_{r2},
      l_{l},
      borderup_{HLine::Through({0, r1_}, {l_, r2_})},
      borderdown_{HLine::Through({0, -r1_}, {l_, -r2_})},
      exit_line_{HLine::Through({l_,r2},{l,-r2})},
      vertical_line_{HLine::Through({0,0},{0,1})},
      horizontal_{1, 0} {
  // TEST: correttezza parametri
  assert(r1_ > 0);
  assert(r2_ > 0);
  assert(l_ > 0);

  //rette parametriche da usare solo per il calcolo delle normali
  Line up{borderup_};
  Line down{borderdown_};

  // Costruzione vettore normale - bordo sup
  const Eigen::Rotation2Df rot_sup{
      -pi / 2};  // rotazione di -pi/2 verso l'interno del biliardo
  normal_up_ = rot_sup * up.direction();

  // Costruzione vettore normale - bordo inf
  const Eigen::Rotation2Df rot_inf{
      pi / 2};  // rotazione di pi/2 verso l'interno del biliardo
  normal_down_ = rot_inf * down.direction();
}

intsect path::operator()(particle const& p) const {
  // CONDIZIONI INIZIALI
  assert(getLocationType(p.pos) == posTypes::Inside ||
         getLocationType(p.pos) ==
             posTypes::BackHit);  // devi essere dentro il biliardo oppure sul bordo
  assert(p.theta >= 0);           // angolo tra 0 e 2pi (convenzione degli angoli)
  assert(p.theta <= 2 * pi + 1e-3); //approx floating point

  const Eigen::Vector2f dir{std::cos(p.theta),
                            std::sin(p.theta)};  // direzione particella

  const Line trajectory{
      p.pos, dir};  // retta della direzione della particella

  //calcola bordo colpito e punto di intersezione
  // casi
  // orizzontale sx: sempre il bordo posteriore
  // orizzontale dx: valutare in base alla coordinata y
  // verticale alto (basso): bordo sup (inf)
  //alto sx: retro o sup
  //basso sx: retro o inf
  //alto dx/basso dx: puo' uscire o colpire uno dei due bordi (inferiore o superiore)
  
  const intsect intersec = [this, &trajectory, &p] () -> intsect {//tipo esplicito
    vecOrientation orientation{getHitDirection(p.theta)}; //orientazione del vettore

    switch (orientation) {
      //se si dire verso destra puo'
      // uscire
      //sbattere sopra o sotto (dipende sia dall'angolo che dal punto in cui si trova la particella)
      case vecOrientation::Right: { 
        //verifica se esce
        const Eigen::Vector2f exit_intsec=exitIntersection(trajectory); //intersezione con la barra di uscita
        if(exit_intsec.y()<r2_+1e-3 && exit_intsec.y()>-r2_-1e-3){
          return {exit_intsec,hitBorder::Front};
        }else if(exit_intsec.y()>=r2_+1e-3){
          return {trajectory.intersectionPoint(
            borderup_),hitBorder::Top};  // intersezione con sup
        }else if(exit_intsec.y()<=-r2_-1e-3){
          return {trajectory.intersectionPoint(
            borderdown_),hitBorder::Bottom};  // intersezione con inf
        }else{
          throw std::logic_error("Impossibile determinare l'intersezione!");
        }
      }

      case vecOrientation::Left:{ //in basso a sx: bordo dietro o basso
          const vec back_intsect{trajectory.intersectionPoint(vertical_line_)}; //intsect con la verticale
          //test intersezione verticale
          if(std::abs(std::abs(back_intsect.y())-r1_)<1e-3){ //colpo all'angolo: distanza da r1 entro i limiti di float
              return {back_intsect,hitBorder::Angle};
          }else if(std::abs(back_intsect.y())<=r1_-1e-3){//colpo al bordo dietro
              return {back_intsect,hitBorder::Back};
          }else if(back_intsect.y()>=r1_+1e-3){//intersezione con il sup
              return {trajectory.intersectionPoint(
                  borderup_), hitBorder::Top};  // intersezione con sup
          }else if(back_intsect.y()<=-r1_-1e-3){
              return {trajectory.intersectionPoint(
                  borderdown_), hitBorder::Bottom};  // intersezione con sup
          }else{
            throw std::logic_error("Impossibile determinare l'intersezione!");
          }
      }

        case vecOrientation::VerticalUp:{
              return {trajectory.intersectionPoint(
                  borderup_), hitBorder::Top};  // intersezione con sup
        }
        case vecOrientation::VerticalDown:{
              return {trajectory.intersectionPoint(
                  borderdown_),hitBorder::Bottom};  // intersezione con inf
        }
    }
    
  }();

  assert(getLocationType(intersec.point) != posTypes::Error);
  
  return intersec;
}

float path::reflect(particle& p) const {
  const intsect intsect = operator()(
      p);  // calcola il punto di collisione
  // determina se è il bordo su o il bordo giù, usando le coordinate del punto
  // di intersezione
  if(intsect.border==hitBorder::Front){ //particella uscita
    p.pos=intsect.point; //punto finale
    return p.theta;
  }else{
  Eigen::Vector2f const& normal_vect = [this,&intsect,&p]() -> vec{
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
  const float normal_angle = arctan(normal_vect.y(), normal_vect.x());
  // vettore traiettoria ribaltato
  const Eigen::Vector2f dir{-std::cos(p.theta), -std::sin(p.theta)};

  const float dir_angle =
      arctan(dir.y(), dir.x());  // calcola angolo del vettore

  assert(dir_angle <= 2 * pi);  // condizioni sulla convenzione degli angoli
  assert(dir_angle >= 0);

  const float phi_inc = normal_angle - dir_angle;  // angolo di incidenza

  // NOTA: phi non deve rispettare la condizione, il segno determina il verso
  // della rotazione
  // const float phi_inc=std::acos(normal.direction().dot(dir));
  // assert(std::abs(phi_inc)<=pi/2);
  // assert al momento non ripristinabile, fornisce problemi di geometria per
  // gli urti con il bordo posteriore risoluzione: prodotto scalare (vettoriale)
  // e con e sin

  const Eigen::Rotation2D rotation{2 *
                                   phi_inc};  // rotazione di 2*angolo incidenza

  const Eigen::Vector2f new_dir = rotation * dir;  // esegui la rotazione

  const float new_angle = arctan(new_dir.y(), new_dir.x());

  p.pos = intsect.point;
  p.theta = new_angle;

  return new_angle;
  }
}

float path::arctan(float y, float x) {
  const float theta = std::atan2f(y, x);
  // correggi l'angolo risultante in modo tale che sia compreso tra zero e 2pi,
  // non tra -pi e pi
  if (theta < 0) {
    return theta + 2 * pi;
  } else {
    return theta;
  }
}

posTypes path::getLocationType(Eigen::Vector2f const& v)
    const {  // determina il luogo del biliardo in cui si trova
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

vecOrientation path::getHitDirection( //determina l'orientazione del vettore
    float angle) const {  // nota: accetta angoli tra -pi e +pi
  assert(angle >= 0 && angle <= 2 * pi);
  
  //assert garantisce che l'angolo sia positivo
  if(angle<pi/2-1e-3){ //alto a dx
    return vecOrientation::Right;
  }else if(std::abs(angle-pi/2)<=1e-3){
    return vecOrientation::VerticalUp;
  }else if(angle>pi/2+1e-3 && angle < 1.5f*pi-1e-3){
    return vecOrientation::Left;
  }else if(std::abs(angle-1.5f*pi)<=1e-3){
    return vecOrientation::VerticalDown;
  }else if(angle>1.5f*pi+1e-3 && angle < 2*pi-1e-3){
    return vecOrientation::Right;
  }else if(std::abs(angle-2*pi)<=1e-3){
    return vecOrientation::Right;
  }else{
    throw std::logic_error("Impossibile determinare la direzione del vettore!");
  }
}

Eigen::Vector2f path::exitIntersection(Line const& l) const{ //intersezione della retta data con quella di uscita
  return l.intersectionPoint(exit_line_);
}

float path::getR1() const{
  return r1_;
}
float path::getR2() const{
  return r2_;
}
float path::getL() const{
  return l_;
}

simulation::simulation(float r1, float r2, float l, int max_cycles)
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
    const Eigen::Vector2f old_pos{p.pos};

    simulator_.reflect(p);  // run the particle reflection

    Eigen::Vector2f const& curr_pos{
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
    return std::make_pair(trajs,exit_point{p.pos.y(),p.theta}); //restituisci la coppia di dati
  }else{
    return std::make_pair(trajs,exit_point{-10,-10}); //restituisci le traiettorie e un indicatore di failure
  }
}

}  // namespace particleSimulator