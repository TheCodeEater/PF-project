#include "../include/particleGenerator.hpp"

namespace particleSimulator {

randSimulator::randSimulator(float pos_mean, float pos_sigma, float angle_mean, float angle_sigma, float r1, unsigned int seed): //constructor
    r1_{r1}, engine_{seed}, pos_dist_{pos_mean,pos_sigma}, angle_dist_{angle_mean,angle_sigma} {}


particle randSimulator::operator()() {//niente const, i generatori cambiano lo stato interno
    //genera angolo - NOTA: angolo tra -pi/2 pi/2
    const float theta=angle_dist_(engine_); 
    const float y=pos_dist_(engine_);

    return particle{{0,y},(theta<0)? 2*pi+theta : theta}; //se l'angolo e' negativo, adatta la convenzione sugli angoli
}

std::normal_distribution<float> const& randSimulator::getPosGenerator() const{
    return pos_dist_;
}
std::normal_distribution<float> const& randSimulator::getAngleGenerator() const{
    return angle_dist_;
}

}