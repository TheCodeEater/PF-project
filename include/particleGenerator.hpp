#ifndef PARTICLE_GENERATOR_HPP
#define PARTICLE_GENERATOR_HPP

#include <random>

#include "traiettoria.hpp"
#include "io.hpp"
namespace particleSimulator {

class randSimulator{ //classe per la simulazione con i numeri casuali

    path simulator_;

    std::default_random_engine engine_; //generatore

    std::normal_distribution<double> pos_dist_; //pdf
    std::normal_distribution<double> angle_dist_;

    public:
        randSimulator(randOptions options); //costruttore
        
        particle getParticle(); //genera le particelle

        std::vector<exit_point> run(int n, int max_iterations); //esegui la simulazione

        std::normal_distribution<double> const& getPosGenerator() const;
        std::normal_distribution<double> const& getAngleGenerator() const;
};

}

#endif