#include "../include/io.hpp"

namespace particleSimulator {
  
namespace fs=std::filesystem;

config::config(){
  //fs::path exe_path{fs::canonical(fs::path{"/proc/self/exe"})}; //get absolute path to current directory
  const fs::path cfg_path{"/cfg/particleSimulator.cfg"};
  //exe_path+=rel_cfg_path;
  //exe_path=fs::canonical(exe_path);
  std::ifstream settings{cfg_path};

  //leggi riga per riga
  while(settings.good()){
    
  }

}

options config::getOptions() const {
  return {2200, 1230,         "Simulatore di particelle", 400, 200, 700,
          300,  std::atanf(2)};
}

options config::getOptions(float y0, float theta0, int N) const {
  return {2200, 1230, "Simulatore di particelle", 400, 200, 700, y0, theta0, N};
}
}  // namespace particleSimulator