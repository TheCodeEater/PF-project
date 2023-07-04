#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#if __INTELLISENSE__
#undef __ARM_NEON
#undef __ARM_NEON__
#endif

#include <SFML/Graphics.hpp>
#include <utility>

#include "../Eigen/Dense"

namespace particleSimulator {

class dottedLine {
  using vType = Eigen::Vector2f;  // tipo di vettore

  vType first_{};
  vType last_{};
  vType direction_{};

  sf::VertexArray vertices_{};

 public:
  dottedLine(vType p0, vType p1, sf::Color c = sf::Color::White);

  // type alias per il tipo di elemento contenuto nei vertex array (vertex)
  typedef sf::Vertex value_type;
  // inserimento dei vertici
  void push_back(value_type);

  // getter per i membri
  sf::VertexArray const& getVertexArray() const;
  std::pair<vType, vType> getExtremes() const;
  Eigen::Vector2f getDirection() const;

  // disegna
  void draw(sf::RenderWindow&) const;
};

}  // namespace particleSimulator

#endif