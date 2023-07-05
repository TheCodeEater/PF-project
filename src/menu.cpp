#include "../include/menu.hpp"

#include <iostream>

#include "../include/math.hpp"

namespace particleSimulator {

void printInit() {
  // cosmetica: bordo sopra
  auto str{
      " -------------------------------------------------------\n"
      " |  Programma di simulazione del biliardo triangolare  |\n"
      " |  Digitare h per una lista di comandi                |\n"
      " -------------------------------------------------------\n"};

  std::cout << str;
}

void printPrompt() {
  std::cout << "> ";  // shell tipo root
}

void printHelp() {
  std::cout << "Uso: <comando> <opzioni> [opzioni]\n";
  std::cout << "s <y0> <theta0> [iterations] : simula la particella con "
               "coordinate date, eseguendo un numero di iterazioni massimo "
               "(opzionale, se omesso usa il valore delle impostazioni)\n\n";
  std::cout
      << "r [iterations] : simula N particelle eseguendo un numero massimo di "
         "iterazioni (se omesso si usa quello delle impostazioni)\n\n";
  std::cout << "q : esci dal programma\n\n";
  std::cout << "h : visualizza questa guida\n\n";
}

void printResult(exit_point const& p_exit) {
  if (std::abs(p_exit.theta + 10) < constants::eps() &&
      std::abs(p_exit.y + 10) < constants::eps()) {
    std::cout << "Particella non uscita. Prova ad aumentare il numero "
                 "di iterazioni\n";
  } else {
    std::cout << "Particella uscita con: Yf=" << p_exit.y
              << " | Phi=" << p_exit.theta << "\n";
  }
}

}  // namespace particleSimulator