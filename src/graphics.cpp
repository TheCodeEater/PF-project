#include "../include/graphics.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
namespace particleSimulator {

dottedLine::dottedLine(vType p0, vType p1, sf::Color c)
    : first_{p0},
      last_{p1},
      direction_{last_ - first_},
      vertices_{sf::Points, 2} {
  // genera punti con un dato step di incremento del parametro finché non superi
  // il punto finale
  const float step{0.01f};
  // calcola la lunghezza della retta
  const double l = direction_.norm();
  // calcola un segmentino
  const vType q{step * direction_};
  // norma del segmento tra punto zero e primo punto
  const double l_1 = q.norm();
  // calcola il numero di punti per rappresentare la retta con il dato step al
  // parametro e arrotonda
  const int N{static_cast<int>(std::round(l / l_1))};
  // genera N punti della linea e inseriscili mediante back inserter
  float t{0.f};
  //Eigen::ParametrizedLine<float,2> line{Eigen::ParametrizedLine<float,2>::Through(first_,last_)};
  std::generate_n(std::back_inserter(*this), N, [this, &t, &step, &c]() {
    // calcola la posizione
    const Eigen::Vector2f pos=first_+t*direction_;
    sf::Vertex vertex{{pos.x(),pos.y()}};
    vertex.color = c;

    t += step;

    return vertex;
  });
}

void dottedLine::draw(sf::RenderWindow& w_) const { w_.draw(vertices_); }

std::pair<dottedLine::vType, dottedLine::vType> dottedLine::getExtremes() const {
  return {first_, last_};
}

sf::VertexArray const& dottedLine::getVertexArray() const { return vertices_; }

void dottedLine::push_back(value_type vertex) { vertices_.append(vertex); }

}  // namespace particleSimulator