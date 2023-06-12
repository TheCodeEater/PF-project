#ifndef PARTICLE_GENERATOR_HPP
#define PARTICLE_GENERATOR_HPP

#include <random>

#include "traiettoria.hpp"
#include "io.hpp"
namespace particleSimulator {

class randSimulator{ //classe per la simulazione con i numeri casuali
    //options
    randOptions optn_{};

    path simulator_;

    std::default_random_engine engine_; //generatore

    std::normal_distribution<float> pos_dist_; //pdf
    std::normal_distribution<float> angle_dist_;

    public:
        randSimulator(randOptions options, unsigned int seed=std::random_device{}()); //costruttore
        
        particle getParticle(); //genera le particelle

        std::vector<particle> run(int n, int max_iterations); //esegui la simulazione

        std::normal_distribution<float> const& getPosGenerator() const;
        std::normal_distribution<float> const& getAngleGenerator() const;
};

}

#endif