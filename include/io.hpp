#ifndef IO_HPP
#define IO_HPP

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>
#include <string>

#include "traiettoria.hpp"
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

struct randOptions {
  float r1{};
  float r2{};
  float l{};

  float pos_mean{};
  float pos_sigma{};
  float angle_mean{};
  float angle_sigma{};

  unsigned seed{};
};

class config {  // classe per caricare le impostazioni
  // impostazioni
  options wOptn_{};  //
  randOptions rOptn_{};
  int N_iter_{};
  int N_particles_{};

 public:
  config();

  options getApplicationOptions(float y0, float theta0, int N) const;
  randOptions const& getRandomOptions() const;
  int getIterations() const;
  int getParticleNumber() const;
  void exportData(std::vector<exit_point> const& v,
                  std::string const& filename) const;
  void exportStatistics(std::vector<exit_point> const& v,
                        std::string const& filename) const;
};
}  // namespace particleSimulator

#endif