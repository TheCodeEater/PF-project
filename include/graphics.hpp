#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include <utility>

namespace particleSimulator {

class dottedLine {

  sf::Vector2f first_{};
  sf::Vector2f last_{};
  sf::Vector2f direction_{};

  sf::VertexArray vertices_{};

 public:
  dottedLine(sf::Vector2f p0, sf::Vector2f p1);

    typedef sf::Vertex value_type;

  /*void setPoints(sf::Vector2f, sf::Vector2f);
  void setPoint1(sf::Vector2f);
  void setPoint2(sf::Vector2f);*/

  //member access
  sf::VertexArray const& getVertexArray() const;
  std::pair<sf::Vector2f,sf::Vector2f> getExtremes() const;

  //drawing
  void draw(sf::RenderWindow&) const;

  //vertex access
  void push_back(value_type);
};

}  // namespace particleSimulator

#endif