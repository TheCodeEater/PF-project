#ifndef TRAIETTORIA_HPP
#define TRAIETTORIA_HPP

#include <SFML/Graphics.hpp>
#include "../Eigen/Dense"
#include <cmath>

using Line=Eigen::Hyperplane<float, 2>;
namespace particleSimulator{
    struct particle{
        Eigen::Vector2f pos{}; //pos sta per position
        double theta{};  //pos e theta da input (distribuzione)
    };
    
    class path {  //contiene i bordi del biliardo
        Line borderup_{};
        Line borderdown_{};
        
        
        public:
        path(Line const&, Line const&);
        Eigen::Vector2f operator()(particle);  // operatore () per calcolare la traiettoria
    
    };
}

#endif