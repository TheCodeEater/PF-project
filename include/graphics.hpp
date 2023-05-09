#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SFML/Graphics.hpp>

namespace particleSimulator{

class dottedLine{
    sf::Vector2f first_{};
    sf::Vector2f last_{};

    sf::VertexArray vertices_{};

    public:
        dottedLine(sf::Vector2f p0,sf::Vector2f p1);

        void setPoints(sf::Vector2f,sf::Vector2f);
        void setPoint1(sf::Vector2f);
        void setPoint2(sf::Vector2f);

        void draw(sf::RenderWindow&) const;
};

}



#endif