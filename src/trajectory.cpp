#include "../include/trajectory.hpp"

#include <cmath>
#include <iostream>

namespace particleSimulator {
path::path(float r1, float r2, float l)
    : r1_{r1},
      r2_{r2},
      l_{l},
      borderup_{HLine::Through({0, r1_}, {l_, r2_})},
      borderdown_{HLine::Through({0, -r1_}, {l_, -r2_})},
      borderback_{HLine::Through({0, 0}, {0, 1})},
      borderfront_{HLine::Through({l_, r2}, {l, -r2})} {
  // TEST: correttezza parametri
  assert(r1_ > 0);
  assert(r2_ > 0);
  assert(l_ > 0);

  assert(r1_ > r2_);

  //linea parametrica dei bordi: necessaria per ottenere il vettore normale
  Line up{borderup_};
  Line down{borderdown_};

  // Costruzione vettore normale - bordo sup
  const Eigen::Rotation2Df rot_sup{
      -bm::pi<float>() / 2};  // rotazione di pi/2 verso l'interno del biliardo
  normal_up_ = rot_sup * up.direction();

  // Vettore normale bordo giu
  const Eigen::Rotation2Df rot_inf{
      bm::pi<float>() / 2};  // rotazione di -pi/2 verso l'interno del biliardo
  normal_down_ = rot_inf * down.direction();
}

intsect path::operator()(particle const& p) const {
  // CONDIZIONI INIZIALI
  assert(getLocationType(p.pos) == posTypes::Inside ||
         std::abs(p.pos.x()) < eps);  // devi essere dentro il biliardo oppure sull'asse Y

  assert(p.theta >= 0);               // angolo tra 0 e 2pi
  assert(p.theta <= 2 * bm::pi<float>());

  const Eigen::Vector2f dir{std::cos(p.theta),
                            std::sin(p.theta)};  // direzione particella

  const Line trajectory{p.pos, dir};  // retta della direzione della particella

  //determina intersezione traiettoria-bordo colpito

  const intsect intersec = [this, &trajectory,
                            &p]() -> intsect {  
    vecOrientation orientation{getHitDirection(p.theta)}; //orientazione del vettore

    switch (orientation) {
      case vecOrientation::Right: {  // direzione destra
        // verifica se esce
        const Eigen::Vector2f exit_intsec{trajectory.intersectionPoint(
            borderfront_)};  // intersezione con la barra di uscita
        if (exit_intsec.y() < r2_ + eps && exit_intsec.y() > -r2_ - eps) {
          return {exit_intsec, hitBorder::Front};
        } else if (exit_intsec.y() >= r2_ + eps) {
          return {trajectory.intersectionPoint(borderup_),
                  hitBorder::Top};  // intersezione con sup
        } else if (exit_intsec.y() <= -r2_ - eps) {
          return {trajectory.intersectionPoint(borderdown_),
                  hitBorder::Bottom};  // intersezione con inf
        } else {
          throw std::logic_error("Impossibile determinare l'intersezione!");
        }
      }

      case vecOrientation::Left: {  // direzione sinistra
        const Eigen::Vector2f back_intsect{trajectory.intersectionPoint(
            borderback_)};  // intsect con la verticale
        // test intersezione verticale
        if (std::abs(back_intsect.y()) <= r1_ + eps) {
          return {back_intsect, hitBorder::Back};
        } else if (back_intsect.y() > r1_ + eps) {
          return {trajectory.intersectionPoint(borderup_), hitBorder::Top};
        } else if (back_intsect.y() < -r1_ - eps) {
          return {trajectory.intersectionPoint(borderdown_), hitBorder::Bottom};
        } else {
          throw std::logic_error("Impossibile determinare l'intersezione");
        }
      }

      case vecOrientation::VerticalUp: {
        return {trajectory.intersectionPoint(borderup_),
                hitBorder::Top};  // intersezione con sup
      }
      case vecOrientation::VerticalDown: {
        return {trajectory.intersectionPoint(borderdown_),
                hitBorder::Bottom};  // intersezione con inf
      }
    }
  }();

  assert(getLocationType(intersec.point) != posTypes::Error); //assicurati che sia possibile determinare il tipo di posizione in cui si trova

  return intersec;
}

void path::reflect(particle& p) const {
  const intsect intsect = operator()(p);  // calcola il punto di collisione
  // determina se è il bordo su o il bordo giù, usando le coordinate del punto
  // di intersezione
  switch (intsect.border) {
    case hitBorder::Front: //colpito pseudobordo frontale: memorizza punto di uscita
      p.pos = intsect.point;
      break;

    case hitBorder::Back: //colpito pseudobordo posteriore: memorizza punto di uscita
      p.pos = intsect.point;
      break;
  //esegui la riflessione se colpisci i bordi veri
    case hitBorder::Top:
      rotate(p, normal_up_, intsect);
      break;

    case hitBorder::Bottom:
      rotate(p, normal_down_, intsect);
      break;

  }  // punto finale
}

void path::rotate(particle& p, Eigen::Vector2f const& normal_vect,
                  intsect const& intersection) const {
  const Line normal{intersection.point, normal_vect};  // trova la retta normale al bordo per il punto di intersezione
  // vettore traiettoria ribaltato
  const Eigen::Vector2f dir{-std::cos(p.theta), -std::sin(p.theta)};

  const float dir_angle =
      arctan(dir.y(), dir.x());  // calcola angolo del vettore

  // condizioni sulla convenzione degli angoli
  assert(dir_angle <=2 * bm::pi<float>());  
  assert(dir_angle >= 0);

  // angolo della normale
  const float normal_angle = arctan(normal_vect.y(), normal_vect.x());

  const float phi_inc = normal_angle - dir_angle;  // angolo di incidenza

  // NOTA: phi non deve rispettare la convenzione sugli angoli, il segno determina il verso
  // della rotazione

  const Eigen::Rotation2D rotation{2 * phi_inc};  // rotazione di 2*angolo incidenza

  const Eigen::Vector2f new_dir = rotation * dir;  // esegui la rotazione

  const float new_angle = arctan(new_dir.y(), new_dir.x()); //determina l'angolo della nuova traiettoria

  p.pos = intersection.point;
  p.theta = new_angle;
}

float arctan(float y, float x) {
  const float theta = std::atan2f(y, x);
  // correggi l'angolo risultante in modo tale che sia compreso tra zero e
  // 2pi, non tra -pi e +pi
  if (theta < 0) {
    return theta + 2 * bm::pi<float>();
  } else {
    return theta;
  }
}

posTypes path::getLocationType(Eigen::Vector2f const& v)
    const {  // determina il luogo del biliardo in cui si trova

  if (v.x() > eps && v.x() <= l_-eps) {  // coordinata x entro i limiti del biliardo
    return posTypes::Inside;
  } else if (v.x() <= eps) {  // x negative: colpisci il fondo
    return posTypes::BackHit;
  } else if (v.x() > l_-eps) {  // fuori: fuggito
    return posTypes::Escaped;
  } else {  // altrimenti, problema
    return posTypes::Error;
  }
}

vecOrientation path::getHitDirection(float const& angle)
    const {  
  //condizione sugli angoli
  assert(angle >= 0 && angle <= 2 * bm::pi<float>());

  if (angle <= eps) {  // angolo nullo: orizzontale destra
    return vecOrientation::Right;
  } else if (angle > eps && angle < bm::pi<float>() / 2 - eps) {  // alto a dx
    return vecOrientation::Right;
  } else if (std::abs(angle - bm::pi<float>() / 2) <= eps) {
    return vecOrientation::VerticalUp;
  } else if (angle > bm::pi<float>() / 2 + eps &&
             angle < bm::pi<float>() - eps) {
    return vecOrientation::Left;
  } else if (std::abs(angle - bm::pi<float>()) <= eps) {
    return vecOrientation::Left;
  } else if (angle > bm::pi<float>() + eps &&
             angle < 1.5f * bm::pi<float>() - eps) {
    return vecOrientation::Left;
  } else if (std::abs(angle - 1.5f * bm::pi<float>()) <= eps) {
    return vecOrientation::VerticalDown;
  } else if (angle > 1.5f * bm::pi<float>() + eps &&
             angle < 2 * bm::pi<float>() - eps) {
    return vecOrientation::Right;
  } else if (std::abs(angle - 2 * bm::pi<float>()) <= eps) {
    return vecOrientation::Right;
  } else {
    throw std::logic_error(
        "Impossibile determinare la direzione del vettore! Angolo: " +
        std::to_string(angle));
  }
}

//definizione dei getter
float path::getR1() const { return r1_; }
float path::getR2() const { return r2_; }
float path::getL() const { return l_; }

bool path::testOutConditions(particle const& p) const {
  //verifica che, in caso in cui la particella sia uscita, la coordinata Y sia corretta
  if (getLocationType(p.pos)==posTypes::BackHit) {  // se esci da dietro, testa con r1
    return p.pos.y() <= r1_ + eps && p.pos.y() >= -r1_ - eps;
  } else if (getLocationType(p.pos)==posTypes::Escaped) {  // da davanti con r2
    return p.pos.y() <= r2_ + eps && p.pos.y() >= -r2_ - eps;
  } else {  // si ha se la particella non esce oppure getLocationType restituisce errore
    return false;
  }
}

simulation::simulation(float r1, float r2, float l, int max_cycles)
    :  // costruttore
      simulator_{r1, r2, l},
      max_iterations_{max_cycles} {}

std::pair<std::vector<dottedLine>, exit_point> simulation::operator()(
    particle& p) const  // operatore di simulazione
{
  p.theta = std::trunc(p.theta * path::trunc_prec) /
            path::trunc_prec;  // troncamento cifre angolo
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

    if (simulator_.getLocationType(p.pos) == posTypes::Escaped ||
        simulator_.getLocationType(p.pos) ==
            posTypes::BackHit) {  // se la particella esce, termina il ciclo
      break;
    }
  }

  // calcolo posizione finale
  if (simulator_.getLocationType(p.pos) == posTypes::Escaped ||
      simulator_.getLocationType(p.pos) == posTypes::BackHit) {
    assert(simulator_.testOutConditions(p));
    return std::make_pair(
        trajs,
        exit_point{p.pos.y(),
                   p.theta});  // restituisci la copbm::pi<float>()a di dati
  } else {
    return std::make_pair(
        trajs,
        exit_point{
            -10,
            -10});  // restituisci le traiettorie e un indicatore di failure
  }
}

std::vector<particle> simulation::getSequence(particle& p,
                                              int max_iterations) const {
  std::vector<particle> pos{};

  p.theta = std::trunc(p.theta * path::trunc_prec) /
            path::trunc_prec;  // troncamento cifre angolo

  for (int i{0}; i < max_iterations;
       ++i) {  // up to the maximum number of iterations
    // position vectors
    const particle old_p{p};

    simulator_.reflect(p);  // run the particle reflection

    pos.push_back(p);

    if (simulator_.getLocationType(p.pos) == posTypes::Escaped ||
        simulator_.getLocationType(p.pos) ==
            posTypes::BackHit) {  // se la particella esce, termina il ciclo
      assert(simulator_.testOutConditions(p));
      break;
    }
  }
  return pos;
}

}  // namespace particleSimulator