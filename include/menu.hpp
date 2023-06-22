#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <iomanip>
#include <string>

namespace particleSimulator{

constexpr std::string_view makeMenuBorder();

void printInit();
void printPrompt();

}


#endif