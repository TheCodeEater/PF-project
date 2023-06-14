#ifndef IO_HPP
#define IO_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <random>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "traiettoria.hpp"
namespace particleSimulator {

  // simulation options
struct options {
  double w_width{};
  double w_height{};
  std::string w_name{};
  // experiment setup
  double r1{};
  double r2{};
  double l{};
  // particle setup
  double y0{};
  double theta0{};
  int N{};
};

struct randOptions{
  double r1{};
  double r2{};
  double l{};

  double pos_mean{};
  double pos_sigma{};
  double angle_mean{};
  double angle_sigma{};

  unsigned seed{};
};

class config {//classe per caricare le impostazioni
  //impostazioni
  options wOptn_{}; //
  randOptions rOptn_{};
  int N_iter_{};
  int N_particles_{};

 public:
  config();

  options getApplicationOptions(double y0, double theta0, int N) const;
  randOptions const& getRandomOptions() const;
  int getIterations() const;
  int getParticleNumber() const;
  void exportData(std::vector<exit_point> const& v,std::string const& filename) const;
};
}  // namespace particleSimulator

#endif