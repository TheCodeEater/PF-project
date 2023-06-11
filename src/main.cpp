#include <iostream>
#include <sstream>
#include <string>

#include "../include/application.hpp"
#include "../include/io.hpp"
#include "../include/particleGenerator.hpp"

// alias for namespace
namespace ps = particleSimulator;

void printInit() {
  std::cout << "    Programma di simulazione biliardo triangolare    \n";
}

void printPrompt() {
  std::cout << "> ";  // shell tipo root
}

int main() {
  ps::config cfg{};

  try {
    // menu
    printInit();
    while (1) {
      printPrompt();
      char in{};
      std::string options{};
      // leggi il comando
      std::cin >> in;
      // leggi le opzioni
      std::getline(std::cin, options);
      // crea lo sstream
      std::istringstream args{options};

      switch (in) {
        case 's': {  // s : simulate. Esegue la simulazione per una particella
                     // con y0 e theta0
          float y0{};
          float theta0{};
          int N{200};  // numero iterazioni di default

          args >> y0;
          if (args.eof()) {
            throw std::runtime_error("Not enough arguments");
          }
          args >> theta0;  // leggi i parametri della particella
          // controlla se vi e il parametro opzionale
          if (!args.eof()) {
            args >> N;
          }

          ps::Application sim{cfg.getOptions(y0, theta0, N)};

          ps::particle p_exit = sim.loop();  // particella all'uscita

          if (std::abs(p_exit.theta) < 1e-3 &&
              p_exit.pos == Eigen::Vector2f{0, 0}) {
            std::cout << "Particella non uscita. Prova ad aumentare il numero "
                         "di iterazioni\n";
          } else {
            std::cout << "Particella uscita con: Yf=" << p_exit.pos.y()
                      << " | Phi=" << p_exit.theta << "\n";
          }

          break;
        }

        case 'r':{//generazione random con output: parametro opzionale: numero di iterazioni
          int N_iter{200};

          if(!args.eof()){
            args>>N_iter;
          }

          ps::path simulator{400,200,700};
          ps::randSimulator rand{0.f,80.f,0.f,pi/4,simulator,10};
          rand.run(100,N_iter);
          break;
        }
        case 'o':{
          break;
        }
        case 'a':{
          break;
        }
        case 'q':{
          return EXIT_SUCCESS;
        }

        default:
          std::cout << "Comando sconosciuto\n";
      }
    }
  } catch (std::runtime_error const e) {
    std::cout << e.what() << "\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cout << "Che cavolo hai combinato?\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}