#include "../include/particleGenerator.hpp"

namespace particleSimulator {

randSimulator::randSimulator(randOptions options): //constructor
     simulator_{options.r1,options.r2,options.l}, 
     engine_{options.seed}, 
     pos_dist_{options.pos_mean,options.pos_sigma}, 
     angle_dist_{options.angle_mean,options.angle_sigma} {}


particle randSimulator::getParticle() {//niente const, i generatori cambiano lo stato interno
    //genera angolo - NOTA: angolo tra -bm::pi<float>()/2 bm::pi<float>()/2
    float theta=angle_dist_(engine_); 
    //test sull'angolo
    if(theta>=bm::pi<float>()/2-1e-3){ //angolo in overflow, poco prima di pi/2
        theta=bm::pi<float>()/2-angle_offset;//pi/2-offset
    }else if(theta<=-bm::pi<float>()/2+1e-3){ //poco prima di -pi/2
        theta=-bm::pi<float>()/2+angle_offset; //-pi/2+offset
    }
    assert(std::abs(theta) < bm::pi<float>()/2 + 1e-3); //test sull'angolo di generazione

    float y=pos_dist_(engine_);

    if(y>=simulator_.getR1()){ //adatta in range la y nel caso
        y=simulator_.getR1()-1e-2;
    }else if(y<=-simulator_.getR1()){
        y=simulator_.getR1()+1e-2;
    }

    const particle p{{0,y},(theta<0)? 2*bm::pi<float>()+theta : theta}; //se l'angolo e' negativo, adatta la convenzione sugli angoli

    return p;
}

std::normal_distribution<float> const& randSimulator::getPosGenerator() const{
    return pos_dist_;
}
std::normal_distribution<float> const& randSimulator::getAngleGenerator() const{
    return angle_dist_;
}

std::vector<exit_point> randSimulator::run(int n,int max_iterations){
    std::vector<particle> particles{};

    //std::function<particle(void)> f{std::bind(&randSimulator::getParticle,this)};

    std::generate_n(std::back_inserter(particles),n,[this](){
        return getParticle();
    }); //esegui la generazione delle particelle

    //vettore dati di uscita
    std::vector<exit_point> exit_p{};
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
            exit_p.push_back(simulator_.getEscapePoint(old_p.pos,p.pos)); 
            break;
        }
        }
    });

    //normalizza l'angolo tra bm::pi<float>()/2 e -bm::pi<float>()/2 per analisi dati ottimale
    std::for_each(exit_p.begin(),exit_p.end(),[](exit_point& p){
        float& phi=p.theta; 
        //tra 3/2 bm::pi<float>() e 2bm::pi<float>()
        if(phi>=1.5f*bm::pi<float>() && phi<=2*bm::pi<float>()+1e-3){
            phi=-(2*bm::pi<float>()-phi);
        }else if(phi>=bm::pi<float>()/2 && phi<=1.5f*phi){
            throw std::runtime_error("Angolo di uscita errato, non esci all'incontrario");
        }
    });

    return exit_p;
}

}