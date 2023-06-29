#include <iostream>
#include <sstream>
#include <string>

#include "../include/application.hpp"
#include "../include/io.hpp"
#include "../include/menu.hpp"
#include "../include/particleGenerator.hpp"

// alias for namespace
namespace ps = particleSimulator;

int main() {
  try {
    const ps::config cfg{};
    // menu
    ps::printInit();
    while (1) {
      ps::printPrompt();
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
          int N{cfg.getIterations()};  // numero iterazioni di default

          args >> y0;
          if (args.eof()) {
            throw std::runtime_error("Not enough arguments");
          }
          args >> theta0;  // leggi i parametri della particella
          // controlla se vi e il parametro opzionale
          if (!args.eof()) {
            args >> N;
          }

          ps::Application sim{cfg.getApplicationOptions(y0, theta0, N)};

          ps::exit_point p_exit = sim.loop();  // particella all'uscita

          if (std::abs(p_exit.theta + 10) < 1e-3 &&
              std::abs(p_exit.y + 10) < 1e-3) {
            std::cout << "Particella non uscita. Prova ad aumentare il numero "
                         "di iterazioni\n";
          } else {
            std::cout << "Particella uscita con: Yf=" << p_exit.y
                      << " | Phi=" << p_exit.theta << "\n";
          }

          break;
        }

        case 'r': {  // generazione random con output: parametro opzionale:
                     // numero di iterazioni
          int N_iter{cfg.getIterations()};

          if (!args.eof()) {
            args >> N_iter;
          }

          ps::randSimulator rand{cfg.getRandomOptions()};
          auto data = rand.run(cfg.getParticleNumber(), N_iter);
          cfg.exportData(data, "output.txt");
          cfg.exportStatistics(data, "statistics.txt");
          break;
        }
        case 'q': {
          return EXIT_SUCCESS;
        }

        case 'h': {
          ps::printHelp();
          break;
        }

        default:
          std::cout << "Comando sconosciuto\n";
      }
    }
  } catch (std::filesystem::filesystem_error const& e) {
    std::cout << e.what() << "\n";
    return EXIT_FAILURE;
  } catch (std::runtime_error const& e) {
    std::cout << e.what() << "\n";
    return EXIT_FAILURE;
  } catch (std::exception const& e) {
    std::cout << e.what() << "\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cout << "Errore sconosciuto\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}