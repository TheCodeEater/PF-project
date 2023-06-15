#include "../include/io.hpp"

#include <iostream>

namespace particleSimulator {
  
namespace fs=std::filesystem;
namespace pt=boost::property_tree;

config::config(){
  const fs::path cfg_path{"cfg/particleSimulator.cfg"};
  pt::ptree tree;

  pt::read_ini(cfg_path,tree);

  //load settings - specifica i valori di default
  wOptn_.w_width=tree.get<float>("width",2200);
  wOptn_.w_height=tree.get<float>("height",1230);
  wOptn_.w_name=tree.get<std::string>("windowTitle","Simulatore di particelle");
  //parametri biliardo
  {
    const double r1=tree.get<double>("R1");
    const double r2=tree.get<double>("R2");
    const double l=tree.get<double>("L");

    wOptn_.r1=r1;
    rOptn_.r1=r1;

    wOptn_.r2=r2;
    rOptn_.r2=r2;

    wOptn_.l=l;
    rOptn_.l=l;
  }

  N_iter_=tree.get<int>("Iterations");
  N_particles_=tree.get<int>("particleNumber");
  rOptn_.seed=tree.get<unsigned>("seed",std::random_device{}());

  rOptn_.pos_mean=tree.get<double>("positionMean");
  rOptn_.pos_sigma=tree.get<double>("positionRMS");
  rOptn_.angle_mean=tree.get<double>("angleMean");
  rOptn_.angle_sigma=tree.get<double>("angleRMS");
  
}

options config::getApplicationOptions(double y0, double theta0, int N) const {
  options opt{wOptn_};
  opt.y0=y0;
  opt.theta0=theta0;
  opt.N=N;
  return opt;
}

randOptions const& config::getRandomOptions() const{
  return rOptn_;
}

int config::getIterations() const{
  return N_iter_;
}

int config::getParticleNumber() const{
  return N_particles_;
}

void config::exportData(std::vector<exit_point> const& v,std::string const& filename) const{
  std::ofstream output{filename};

  if(!output.is_open()){
    throw std::runtime_error("Impossibile creare file di output!");
  }

  std::for_each(v.cbegin(),v.cend(),[&output](exit_point const& p){
      std::ostringstream line{};
      line<<p.y<<" "<<p.theta<<"\n";
      output<<line.str(); //scrivi la stringa
  });
}

}  // namespace particleSimulator