#ifndef IO_HPP
#define IO_HPP

#include "application.hpp"
#include <filesystem>
#include <fstream>
namespace particleSimulator {

class config {//classe per caricare le impostazioni
 public:
  config();
  options getOptions() const;
  options getOptions(float y0, float theta0, int N) const;
};
}  // namespace particleSimulator

#endif