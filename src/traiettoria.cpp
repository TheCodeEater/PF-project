#include "../include/traiettoria.hpp"

#include <cmath>
#include <iostream>

namespace particleSimulator {
path::path(float r1, float r2, float l)
    : r1_{r1},
      r2_{r2},
      l_{l},
      borderup_{Line::Through({0, r1_}, {l_, r2_})},
      borderdown_{Line::Through({0, -r1_}, {l_, -r2_})},
      exit_line_{Eigen::Hyperplane<float,2>::Through({l_,r2},{l,-r2})},
      horizontal_{1, 0} {
  // TEST: correttezza parametri
  assert(r1_ > 0);
  assert(r2_ > 0);
  assert(l_ > 0);
  // TEST: correttezza della direzione dei vettori
  assert(borderup_.direction().x() > 0);
  assert(borderup_.direction().y() < 0);  // test bordo sopra
  assert(borderdown_.direction().x() > 0);
  assert(borderdown_.direction().y() > 0);  // test bordo sotto

  // Costruzione vettore normale - bordo sup
  const Eigen::Rotation2Df rot_sup{
      -pi / 2};  // rotazione di -pi/2 verso l'interno del biliardo
  normal_up_ = rot_sup * borderup_.direction();

  // Costruzione vettore normale - bordo inf
  const Eigen::Rotation2Df rot_inf{
      pi / 2};  // rotazione di pi/2 verso l'interno del biliardo
  normal_down_ = rot_inf * borderdown_.direction();
}

intsect path::operator()(particle const& p) const {
  // CONDIZIONI INIZIALI
  assert(getLocationType(p.pos) == posTypes::Inside ||
         getLocationType(p.pos) ==
             posTypes::BackHit);  // devi essere dentro il biliardo oppure sul bordo
  assert(p.theta >= 0);           // angolo tra 0 e 2pi (convenzione degli angoli)
  assert(p.theta <= 2 * pi);

  const Eigen::Vector2f dir{std::cos(p.theta),
                            std::sin(p.theta)};  // direzione particella

  const Eigen::ParametrizedLine<float, 2> trajectory{
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
      case vecOrientation::HorizontalLeft: {  // sbatti sempre sul bordo posteriore
        return {trajectory.intersectionPoint(
            HLine{HLine::Through({0, 0}, {0, 1})}),hitBorder::Back};
      }
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
            HLine{borderup_}),hitBorder::Top};  // intersezione con sup
        }else if(exit_intsec.y()<=-r2_-1e-3){
          return {trajectory.intersectionPoint(
            HLine{borderdown_}),hitBorder::Bottom};  // intersezione con inf
        }else{
          throw std::logic_error("Impossibile determinare l'intersezione!");
        }
      }

      case vecOrientation::UpLeft:{ //in basso a sx: bordo dietro o basso
          const Eigen::Vector2f back_intsect{trajectory.intersectionPoint(Eigen::Hyperplane<float,2>::Through({0,0},{0,1}))}; //intsect con la verticale
          //test intersezione verticale
          if(std::abs(std::abs(back_intsect.y())-r1_)<1e-3){ //colpo all'angolo: distanza da r1 entro i limiti di float
              return {back_intsect,hitBorder::Angle};
          }else if(std::abs(back_intsect.y())<=r1_-1e-3){//colpo al bordo dietro
              return {back_intsect,hitBorder::Back};
          }else if(std::abs(back_intsect.y())>=r1_+1e-3){//intersezione con il sup
              return {trajectory.intersectionPoint(
                  Eigen::Hyperplane<float, 2>{borderup_}), hitBorder::Top};  // intersezione con sup
          }
      }
      case vecOrientation::DownLeft:{ //in alto a sx: bordo dietro o alto
          const Eigen::Vector2f back_intsect{trajectory.intersectionPoint(Eigen::Hyperplane<float,2>::Through({0,0},{0,1}))}; //intsect con la verticale
          //test intersezione verticale
          if(std::abs(std::abs(back_intsect.y())-r1_)<1e-3){ //colpo all'angolo
              return {back_intsect,hitBorder::Angle};
          }else if(std::abs(back_intsect.y())<=r1_-1e-3){//colpo al bordo dietro
              return {back_intsect,hitBorder::Back};
          }else if(std::abs(back_intsect.y())>=r1_+1e-3){//intersezione con inf
              return {trajectory.intersectionPoint(
                  Eigen::Hyperplane<float, 2>{borderdown_}),hitBorder::Bottom};  // intersezione con inf
          }
      }

        case vecOrientation::VerticalUp:{
              return {trajectory.intersectionPoint(
                  Eigen::Hyperplane<float, 2>{borderup_}), hitBorder::Top};  // intersezione con sup
        }
        case vecOrientation::VerticalDown:{
              return {trajectory.intersectionPoint(
                  Eigen::Hyperplane<float, 2>{borderdown_}),hitBorder::Bottom};  // intersezione con inf
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
  // assert; verifica che la x di v corrisponda al corrispondente valore Y
  // calcolato
  if (v.x() > 0 && v.x() < l_) {  // coordinata x entro i limiti del biliardo
    return posTypes::Inside;
  /*}else if(std::abs(v.x())<1e-2 && (v.y()>r1_ || v.y()<-r1_)){ //errore se hai la y fuori posto
    std::cout<<"Posizione incriminata: "<<v<<"\n";
    return posTypes::Error;*/
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
  
  if(angle<=1e-3){ //angolo nullo: orizzontale destra
    return vecOrientation::Right;
  }else if(angle>1e-3 && angle<pi/2-1e-3){ //alto a dx
    return vecOrientation::Right;
  }else if(std::abs(angle-pi/2)<=1e-3){
    return vecOrientation::VerticalUp;
  }else if(angle>pi/2+1e-3 && angle<pi-1e-3){
    return vecOrientation::UpLeft;
  }else if(std::abs(angle-pi)<=1e-3){
    return vecOrientation::HorizontalLeft;
  }else if(angle>pi+1e-3 && angle < 1.5f*pi-1e-3){
    return vecOrientation::DownLeft;
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
    return std::make_pair(trajs,simulator_.getEscapePoint(trajs)); //restituisci la coppia di dati
  }else{
    return std::make_pair(trajs,exit_point{-10,-10}); //restituisci le traiettorie e un indicatore di failure
  }
}

exit_point path::getEscapePoint(vec const& p0, vec const& p1) const{
      //test che sia effettivamente fuggita
      assert(getLocationType(p1)==posTypes::Escaped);

      const Eigen::ParametrizedLine<float,2> line{Eigen::ParametrizedLine<float,2>::Through(p0,p1)};

      //calcolo Y: interseca con asse di uscita
      const Eigen::Vector2f escape_intersection{exitIntersection(line)}; //ottieni punto di fuga
      //test intersezo
      assert(escape_intersection.y()<=getR2());
      assert(escape_intersection.y()>=-getR2());

      const float escape_phi{std::atanf(line.direction().y()/line.direction().x())}; //angolo di uscita tra -pi/2 e pi/2

      return {escape_intersection.y(),escape_phi}; //restituisci il punto di fuga
}

  exit_point path::getEscapePoint(std::vector<dottedLine> const& trajectiories) const{
      //to do: aggiungere dei getter a path cosi da poter fare i test con gli assert
      //la funzione assume che la particella sia fuggita
      dottedLine const& last=trajectiories.back(); //ottieni traiettoria di
      const auto extremes=last.getExtremes();
      const Eigen::Vector2f p0=extremes.first;
      const Eigen::Vector2f p1=extremes.second;

      return getEscapePoint(p0,p1);

  }

}  // namespace particleSimulator