#include "../include/graphics.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>
namespace particleSimulator {

sf::Vector2f operator*(float t, sf::Vector2f v){
    return {t*v.x,t*v.y};
}

dottedLine::dottedLine(sf::Vector2f p0, sf::Vector2f p1)
    : first_{p0}, last_{p1}, direction_{last_-first_}, vertices_{sf::Points, 2} {
        //genera N punti della linea e inseriscili mediante back inserter
        int i{};
        std::generate_n(std::back_inserter(*this),20,[this,&i](){
            //calcola la posizione
            sf::Vertex vertex{0.1*(first_+i*direction_)};
            ++i;

            return vertex;
        });
    }

void dottedLine::draw(sf::RenderWindow& w_) const { w_.draw(vertices_); }

std::pair<sf::Vector2f,sf::Vector2f> dottedLine::getExtremes() const{
    return {first_,last_};
}

sf::VertexArray const& dottedLine::getVertexArray() const{
    return vertices_;
}

void dottedLine::push_back(value_type vertex){
    vertices_.append(vertex);
}

}  // namespace particleSimulator