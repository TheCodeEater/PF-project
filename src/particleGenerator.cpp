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

void randSimulator::run(int n,int max_iterations){
    std::vector<particle> particles{};

    //std::function<particle(void)> f{std::bind(&randSimulator::getParticle,this)};

    std::generate_n(std::back_inserter(particles),n,[this](){
        return getParticle();
    }); //esegui la generazione delle particelle

    //loop di calcolo
    std::for_each(particles.begin(),particles.end(),[this,&max_iterations](particle& p){
        //esegui la riflessione
    for (int i{0}; i < max_iterations;
        ++i) {  // up to the maximum number of iterations
        // position vectors
        const Eigen::Vector2f old_pos{p.pos};

        simulator_.reflect(p);  // run the particle reflection

        Eigen::Vector2f const& curr_pos{
            p.pos};  // const reference to current position

        if (simulator_.getLocationType(p.pos) ==
            posTypes::Escaped) {  // se la particella esce, termina il ciclo
            break;
        }
        }
    });
}

}