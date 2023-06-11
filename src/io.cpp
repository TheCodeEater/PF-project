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
  /*
  //leggi riga per riga
  while(cfg_file.good()){
    std::string line{};
    std::getline(cfg_file,line);
    //ottieni lo stringstream
    std::istringstream settings{line};

    //ignora spazi bianchi
    settings>>std::ws;

    const char first=settings.peek();
    if(first!='#' && std::isalnum(first)){//continua, altrimenti non fai nulla e passi al prossimo ciclo
      std::cout<<line.c_str()<<"\n";

      //lettura fino a uguale
      std::string init{};
      std::getline(settings,init,'='); //prendi fino ad uguale

      //assegna la variabile
      if(init=="I:\"width\""){

      }
      
    }
  }*/

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