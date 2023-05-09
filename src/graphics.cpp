#include "../include/graphics.hpp"

namespace particleSimulator {
dottedLine::dottedLine(sf::Vector2f p0, sf::Vector2f p1)
    : first_{p0}, last_{p0}, vertices_{sf::Points, 2} {}

void dottedLine::draw(sf::RenderWindow& w_) const { w_.draw(vertices_); }
}  // namespace particleSimulator