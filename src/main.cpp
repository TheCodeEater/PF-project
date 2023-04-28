#include "../include/application.hpp"

//alias for namespace
namespace ps=particleSimulator;

int main(){
    ps::Application app{};

    return app.loop();
}