#include "../include/particleGenerator.hpp"

namespace particleSimulator {

randSimulator::randSimulator(float pos_mean, float pos_sigma, float angle_mean, float angle_sigma): //constructor
    engine_{std::random_device{}()}, pos_dist_{pos_mean,pos_sigma}, angle_dist_{angle_mean,angle_sigma} {}

}