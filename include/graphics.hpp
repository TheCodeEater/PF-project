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
  using vType = Eigen::Vector2f;  // vetor type

  vType first_{};
  vType last_{};
  vType direction_{};

  sf::VertexArray vertices_{};

 public:
  dottedLine(vType p0, vType p1, sf::Color c = sf::Color::White);

  typedef sf::Vertex value_type;

  /*void setPoints(sf::Vector2f, sf::Vector2f);
  void setPoint1(sf::Vector2f);
  void setPoint2(sf::Vector2f);*/

  // member access
  sf::VertexArray const& getVertexArray() const;
  std::pair<vType, vType> getExtremes() const;
  Eigen::Vector2f getDirection() const;

  // drawing
  void draw(sf::RenderWindow&) const;

  // vertex access
  void push_back(value_type);
};

}  // namespace particleSimulator

#endif