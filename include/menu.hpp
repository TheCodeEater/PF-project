#ifndef MENU_HPP
#define MENU_HPP

#include <iomanip>
#include <iostream>
#include <string>

namespace particleSimulator {

constexpr std::string_view makeMenuBorder();

void printInit();
void printPrompt();

}  // namespace particleSimulator

#endif