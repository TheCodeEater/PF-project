#include "../include/graphics.hpp"

#include <algorithm>
#include <cmath>
namespace particleSimulator {

// costruttore: delega la costruzione a quello di vertex array
VArray::VArray(sf::PrimitiveType type, std::size_t vertexCount)
    : sf::VertexArray(type, vertexCount) {}

void VArray::push_back(value_type vertex) {
  append(vertex);
}  // richiama la funzione append. Necessaria poiché back_inserter richiede
   // push_back, non append

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
  std::generate_n(std::back_inserter(vertices_), N, [this, &t, &step, &c]() {
    // calcola la posizione
    const Eigen::Vector2f pos = first_ + t * direction_;
    sf::Vertex vertex{{pos.x(), pos.y()}};
    vertex.color = c;

    t += step;

    return vertex;
  });
}

void dottedLine::draw(sf::RenderWindow& w_) const { w_.draw(vertices_); }

}  // namespace particleSimulator