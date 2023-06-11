#include "../include/particleGenerator.hpp"

namespace particleSimulator {

randSimulator::randSimulator(float pos_mean, float pos_sigma, float angle_mean, float angle_sigma, path const& simul, unsigned int seed): //constructor
     simulator_{simul}, engine_{seed}, pos_dist_{pos_mean,pos_sigma}, angle_dist_{angle_mean,angle_sigma} {}


particle randSimulator::getParticle() {//niente const, i generatori cambiano lo stato interno
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

std::vector<particle> randSimulator::run(int n,int max_iterations){
    std::vector<particle> particles{};

    //std::function<particle(void)> f{std::bind(&randSimulator::getParticle,this)};

    std::generate_n(std::back_inserter(particles),n,[this](){
        return getParticle();
    }); //esegui la generazione delle particelle

    //vettore dati di uscita
    std::vector<particle> exit_p{};
    //loop di calcolo
    std::for_each(particles.begin(),particles.end(),[this,&max_iterations,&exit_p](particle& p){
        //esegui la riflessione
    for (int i{0}; i < max_iterations;
        ++i) {  // up to the maximum number of iterations
        // position vectors
        const particle old_p{p};

        simulator_.reflect(p);  // run the particle reflection

        if (simulator_.getLocationType(p.pos) ==
            posTypes::Escaped) {  // se la particella esce, termina il ciclo
            exit_p.push_back(old_p);
            break;
        }
        }
    });

    //normalizza l'angolo tra pi/2 e -pi/2 per analisi dati ottimale
    std::transform(exit_p.begin(),exit_p.end(),exit_p.begin(),[](particle p){
        if(p.theta>=1.5f*pi){
            return particle{p.pos,-(2*pi-p.theta)};
        }else{
            return p;
        }
    });

    return exit_p;
}

}