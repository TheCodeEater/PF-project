#include "../include/traiettoria.hpp"
#include <cmath>

namespace particleSimulator{
path::path(sf::VertexArray const& bup, sf::VertexArray const& bdown): borderup_{bup}, borderdown_{bdown}{}

Eigen::Vector2f path::operator()(particle p){
    Eigen::Vector2f dir{std::cos(p.theta), std::sin(p.theta)};
    }



}