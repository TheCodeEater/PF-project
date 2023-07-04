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
  try {  // cattura eventuali eccezioni

    bool doRun{true};        // flag per eseguire il loop
    const ps::config cfg{};  // oggetto di configurazione

    // stampa menu iniziale
    ps::printInit();

    while (doRun) {
      ps::printPrompt();  // stampa il prompt
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

          ps::Application sim{cfg.getApplicationOptions(
              y0, theta0, N)};  // inizializza la simulazione

          ps::exit_point p_exit =
              sim.loop();  // esegui la simulazione, disegnando. Alla chiusura
                           // della finestra, restituisce il punto di uscita
          printResult(p_exit); //stampa i risultati

          break;
        }

        case 'r': {  // generazione random con output: parametro opzionale:
                     // numero di iterazioni
          int N_iter{cfg.getIterations()};

          if (!args.eof()) {  // se vi è un ulteriore argomento, leggilo (se non
                              // è int, da errore)
            args >> N_iter;
          }

          ps::randSimulator rand{
              cfg.getRandomOptions()};  // inizializza il simulatore
          auto data = rand.run(cfg.getParticleNumber(),
                               N_iter);  // esegui la simulazione

          // esporta i dati
          cfg.exportData(data, "output.txt");
          cfg.exportStatistics(data, "statistics.txt");
          break;
        }
        case 'q': {
          doRun = false;  // imposta il flag di terminazione del loop
        }

        case 'h': {
          ps::printHelp();  // stampa l'aiuto
          break;
        }

        default:
          std::cout << "Comando sconosciuto\n";
      }
    }
  } catch (
      std::filesystem::filesystem_error const& e) {  // gestisci le eccezioni
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