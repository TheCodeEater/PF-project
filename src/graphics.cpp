#include "../include/graphics.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
namespace particleSimulator {

sf::Vector2f operator*(float t, sf::Vector2f const& v) {
  return {t * v.x, t * v.y};
}

sf::Vector2f operator*(sf::Vector2f const& v, float t) {
  return {t * v.x, t * v.y};
}

dottedLine::dottedLine(sf::Vector2f p0, sf::Vector2f p1, sf::Color c)
    : first_{p0},
      last_{p1},
      direction_{last_ - first_},
      vertices_{sf::Points, 2} {
  // genera punti con un dato step di incremento del parametro finché non superi
  // il punto finale
  const float step{0.2f};
  // calcola la lunghezza della retta
  const double l =
      std::sqrt(direction_.x * direction_.x + direction_.y * direction_.y);
  // calcola un segmentino
  const sf::Vector2f q{step * direction_};
  // norma del segmento tra punto zero e primo punto
  const double l_1 = std::sqrt(q.x * q.x + q.y * q.y);
  // calcola il numero di punti per rappresentare la retta con il dato step al
  // parametro e arrotonda
  const int N{static_cast<int>(std::round(l / l_1))};
  // genera N punti della linea e inseriscili mediante back inserter
  float t{0.f};
  std::generate_n(std::back_inserter(*this), N, [this, &t, &step, &c]() {
    // calcola la posizione
    sf::Vertex vertex{first_ + t * direction_};
    vertex.color = c;

    t += step;

    return vertex;
  });
}

void dottedLine::draw(sf::RenderWindow& w_) const { w_.draw(vertices_); }

std::pair<sf::Vector2f, sf::Vector2f> dottedLine::getExtremes() const {
  return {first_, last_};
}

sf::VertexArray const& dottedLine::getVertexArray() const { return vertices_; }

void dottedLine::push_back(value_type vertex) { vertices_.append(vertex); }

}  // namespace particleSimulator