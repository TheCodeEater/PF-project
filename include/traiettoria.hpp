#ifndef TRAIETTORIA_HPP
#define TRAIETTORIA_HPP

#include <SFML/Graphics.hpp>
#include "../Eigen/Dense"
#include <cmath>

namespace particleSimulator{
    struct particle{
        Eigen::Vector2f pos{}; //pos sta per position
        double theta{};
    };
    
    class path {
        sf::VertexArray const& borderup_{};
        sf::VertexArray const& borderdown_{};
        
        
        public:
        path(sf::VertexArray const&, sf::VertexArray const&);
        Eigen::Vector2f operator()(particle);
    
    };
}

#endif