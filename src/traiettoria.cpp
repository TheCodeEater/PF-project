#include "../include/traiettoria.hpp"
#include <cmath>

namespace particleSimulator{
path::path(Line const& bup, Line const& bdown): borderup_{bup}, borderdown_{bdown}{}

Eigen::Vector2f path::operator()(particle p){
    Eigen::Vector2f dir{std::cos(p.theta), std::sin(p.theta)};
    //tentativi falliti
    //Eigen::Matrix2f sist1{dir, borderup_};
    //Eigen::Matrix2f sist2{dir, borderdown_};
    Line pippo{p.pos, dir}; //pippo è TEMPORANEO
    auto intersection = pippo.intersection(borderup_);
    return intersection;
    }



}