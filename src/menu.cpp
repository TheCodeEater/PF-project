#include "../include/menu.hpp"

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
}  // namespace particleSimulator