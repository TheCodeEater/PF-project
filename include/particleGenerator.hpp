#ifndef PARTICLE_GENERATOR_HPP
#define PARTICLE_GENERATOR_HPP

#include <random>

#include "traiettoria.hpp"
namespace particleSimulator {

class randSimulator{ //classe per la simulazione con i numeri casuali
    float r1_{};

    std::default_random_engine engine_; //generatore

    std::normal_distribution<float> pos_dist_; //pdf
    std::normal_distribution<float> angle_dist_;

    public:
        randSimulator(float pos_mean, float pos_sigma, float angle_mean, float angle_sigma, float r1, float seed=std::random_device{}()); //costruttore
        
        particle operator()(); //genera le particelle

        std::normal_distribution<float> const& getPosGenerator() const;
        std::normal_distribution<float> const& getAngleGenerator() const;
};

}

#endif