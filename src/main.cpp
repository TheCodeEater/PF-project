#include "../include/application.hpp"

//alias for namespace
namespace ps=particleSimulator;

int main(){
    ps::options initial_config{2200,1230,"Simulatore di particelle",400,300,700};
    ps::Application app{initial_config};

    return app.loop();
}