#ifndef IO_HPP
#define IO_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
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

struct randOptions{
  float r1{};
  float r2{};
  float l{};

  float pos_mean{};
  float pos_sigma{};
  float angle_mean{};
  float angle_sigma{};
};

class config {//classe per caricare le impostazioni
  //impostazioni
  options wOptn_{}; //
  randOptions rOptn_{};
  int N_iter_{};

 public:
  config();
  options const& getApplicationOptions(float y0, float theta0, int N) const;
  randOptions const& getRandomOptions() const;
  int getIterations() const;
};
}  // namespace particleSimulator

#endif