#include "../include/io.hpp"

#include <iostream>

namespace particleSimulator {
  
namespace fs=std::filesystem;

config::config(){
  //fs::path exe_path{fs::canonical(fs::path{"/proc/self/exe"})}; //get absolute path to current directory
  const fs::path cfg_path{"cfg/particleSimulator.cfg"};
  //exe_path+=rel_cfg_path;
  //exe_path=fs::canonical(exe_path);
  std::ifstream cfg_file{cfg_path};

  if(!cfg_file.is_open()){
    throw std::runtime_error("Cannot open config file!");
  }
}

options const& config::getApplicationOptions(float y0, float theta0, int N) const {
  return wOptn_;
}

randOptions const& config::getRandomOptions() const{
  return rOptn_;
}

int config::getIterations() const{
  return N_iter_;
}
}  // namespace particleSimulator