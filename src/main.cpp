#include "../include/application.hpp"
#include "../include/io.hpp"

#include <iostream>
#include <sstream>
#include <string>

// alias for namespace
namespace ps = particleSimulator;

void printInit(){
  std::cout<<"    Programma di simulazione biliardo triangolare    \n";
}

void printPrompt(){
  std::cout<<"> "; //shell tipo root
}

int main() {
  ps::config cfg{};
  /*ps::Application app{c.getOptions()};

  return app.loop();*/

  //menu
  printInit();
  while(1){
      printPrompt();
      char in{};
      std::string options{};
      //leggi il comando
      std::cin>>in;
      //leggi le opzioni
      std::getline(std::cin,options);
      //crea lo sstream
      std::istringstream args{options};

      switch(in){
        case 's': {//s : simulate. Esegue la simulazione per una particella con y0 e theta0
            float y0{};
            float theta0{};
            int N{200};//numero iterazioni di default

            args>>y0>>theta0; //leggi i parametri della particella
            //controlla se vi e il parametro opzionale
            if(!args.eof()){
              args>>N;
            }

            ps::Application sim{cfg.getOptions(y0,theta0,N)};

            ps::particle p_exit=sim.loop(); //particella all'uscita

            std::cout<<"Particella uscita con: xf="<<p_exit.pos.x()<<" | Phi="<<p_exit.theta<<"\n";

          break;
        }
        case 'q':
          return 0;

        default:
          std::cout<<"Comando sconosciuto\n";
      }
      
  }
}