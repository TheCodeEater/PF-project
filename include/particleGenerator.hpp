#ifndef PARTICLE_GENERATOR_HPP
#define PARTICLE_GENERATOR_HPP

#include <random>

#include "traiettoria.hpp"
namespace particleSimulator {

class randSimulator{ //classe per la simulazione con i numeri casuali
    path simulator_;

    std::default_random_engine engine_; //generatore

    std::normal_distribution<float> pos_dist_; //pdf
    std::normal_distribution<float> angle_dist_;

    public:
        randSimulator(float pos_mean, float pos_sigma, float angle_mean, float angle_sigma, path const& simul, unsigned int seed=std::random_device{}()); //costruttore
        
        particle getParticle(); //genera le particelle

        std::vector<particle> run(int n, int max_iterations); //esegui la simulazione

        std::normal_distribution<float> const& getPosGenerator() const;
        std::normal_distribution<float> const& getAngleGenerator() const;
};

}

#endif