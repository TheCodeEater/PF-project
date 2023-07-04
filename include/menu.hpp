#ifndef MENU_HPP
#define MENU_HPP

#include "../include/trajectory.hpp"
namespace particleSimulator {

void printInit();    // stampa schermata iniziale
void printPrompt();  // stampa shell di prompt
void printHelp();    // stampa aiuto
void printResult(exit_point const& p_exit);

}  // namespace particleSimulator

#endif