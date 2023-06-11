#ifndef IO_HPP
#define IO_HPP

#include <filesystem>
#include <fstream>
namespace particleSimulator {

  // simulation options
struct options {
  float w_width{};
  float w_height{};
  std::string w_name{};
  // experiment setup
  float r1{};
  float r2{};
  float l{};
  // particle setup
  float y0{};
  float theta0{};
  int N{};
};

class config {//classe per caricare le impostazioni

 public:
  config();
  options getApplicationOptions(float y0, float theta0, int N) const;
};
}  // namespace particleSimulator

#endif