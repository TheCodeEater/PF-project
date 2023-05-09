#include "../include/graphics.hpp"

#include <cmath>
#include <iostream>
namespace particleSimulator {

sf::Vector2f operator*(float t, sf::Vector2f v){
    return {t*v.x,t*v.y};
}

dottedLine::dottedLine(sf::Vector2f p0, sf::Vector2f p1)
    : first_{p0}, last_{p1}, vertices_{sf::Points, 2} {
        //crea i punti intermedi
        //usa uno step
        const int step{10};
        const sf::Vector2f v{p1-p0}; //vettore direttore della retta
        auto r=[p0,v](int t){
            return 0.1*(p0+t*v);
        };
        for(int i{};i<20;++i){
            sf::Vertex vert{r(i)};
            vert.color=sf::Color::Red;
            vertices_.append(vert);
        }
    }

void dottedLine::draw(sf::RenderWindow& w_) const { w_.draw(vertices_); }

std::pair<sf::Vector2f,sf::Vector2f> dottedLine::getExtremes() const{
    return {first_,last_};
}

sf::VertexArray const& dottedLine::getVertexArray() const{
    return vertices_;
}

}  // namespace particleSimulator