#ifndef PARTICLE_GENERATOR_HPP
#define PARTICLE_GENERATOR_HPP

#include <random>
namespace particleSimulator {

class randSimulator{ //classe per la simulazione con i numeri casuali
    std::default_random_engine engine_;

    std::normal_distribution<float> pos_dist_;
    std::normal_distribution<float> angle_dist_;

    public:
        randSimulator(float pos_mean, float pos_sigma, float angle_mean, float angle_sigma);
};

}

#endif