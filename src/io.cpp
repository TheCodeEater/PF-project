#include "../include/io.hpp"

#include <filesystem>
#include <fstream>
#include <random>

#include "../include/statistics.hpp"
#include "../include/typeAlias.hpp"
#include "../include/dataStructures.hpp"

namespace particleSimulator {

namespace fs = std::filesystem;
namespace pt = boost::property_tree;

randOptions::randOptions(float r1, float r2, float l, float p_mean,
                         float p_sigma, float a_mean, float a_sigma,
                         unsigned seed, bool genRSeed = false)
    : r1{r1},
      r2{r2},
      l{l},
      pos_mean{p_mean},
      pos_sigma{p_sigma},
      angle_mean{a_mean},
      angle_sigma{a_sigma},
      seed{seed},
      randomSeed{genRSeed} {}

randOptions::randOptions(randOptions const& rData, unsigned rSeed)
    : randOptions{rData} {  // delega copia al copy constructor
  seed = rSeed;             // imposta il seme
}

config::config() {
  const fs::path cfg_path{"cfg/particleSimulator.cfg"};  // percorso al file
  pt::ptree tree;  // struttura dati per le impostazioni

  pt::read_ini(cfg_path, tree);  // parse del file

  // load settings - specifica i valori di default
  wOptn_.w_width = tree.get<int>("width", 2200);
  wOptn_.w_height = tree.get<int>("height", 1230);
  wOptn_.w_name =
      tree.get<std::string>("windowTitle", "Simulatore di particelle");
  // parametri biliardo
  {
    const float r1 = tree.get<float>("R1");
    const float r2 = tree.get<float>("R2");
    const float l = tree.get<float>("L");

    wOptn_.r1 = r1;
    rOptn_.r1 = r1;

    wOptn_.r2 = r2;
    rOptn_.r2 = r2;

    wOptn_.l = l;
    rOptn_.l = l;
  }

  N_iter_ = tree.get<int>("Iterations");
  N_particles_ = tree.get<int>("particleNumber");

  // seed
  {
    int seed = tree.get<int>(
        "seed", -1);  // leggi il seed. Se non lo trovi, restituisci -1

    if (seed == -1) {  // se non vi è seme, abilita seme casuale
      rOptn_.randomSeed = true;
    } else {  // altrimenti, salvalo
      rOptn_.randomSeed = false;
      rOptn_.seed = seed;
    }
  }

  rOptn_.pos_mean = tree.get<float>("positionMean");
  rOptn_.pos_sigma = tree.get<float>("positionRMS");
  rOptn_.angle_mean = tree.get<float>("angleMean");
  rOptn_.angle_sigma = tree.get<float>("angleRMS");
}

options config::getApplicationOptions(float y0, float theta0, int N)
    const {  // costruisci struct impostazioni della simulazione grafica,
             // aggiungendo i parametri non letti da file
  options opt{wOptn_};
  opt.y0 = y0;
  opt.theta0 = theta0;
  opt.N = N;
  return opt;
}

randOptions config::getRandomOptions() const {
  if (rOptn_.randomSeed) {  // genera seed se abilitato
    return randOptions{rOptn_,
                       std::random_device{}()};  // crea struct con random seed
  } else {
    return rOptn_;
  }

  return rOptn_;
}  // ottieni parametri random generator

int config::getIterations() const {
  return N_iter_;
}  // restituisce numero max di iterazioni

int config::getParticleNumber() const {
  return N_particles_;
}  // restituisce numero di particelle da simulare

void config::exportData(std::vector<exit_point> const& v,
                        std::string const& filename)
    const {  // scrivi i dati dei punti di uscita su file
  std::ofstream output{"data/" + filename};

  if (!output.is_open()) {
    throw std::runtime_error("Impossibile creare file di output!");
  }
  // scrivi i parametri della simulazione
  output << wOptn_.r1 << "\n";

  // scrivi i dati
  std::for_each(v.cbegin(), v.cend(), [&output](exit_point const& p) {
    std::ostringstream line{};
    line << p.y << " " << p.theta << "\n";
    output << line.str();  // scrivi la stringa
  });
}

void config::exportStatistics(std::vector<exit_point> const& v,
                              std::string const& filename) const {
  std::ofstream output{"data/" + filename};  // apri file di output

  if (!output.is_open()) {  // test della corretta apertura
    throw std::runtime_error("Impossibile creare file di output! (statistics)");
  }

  // calcoli statistici - COORDINATA DI USCITA
  {
    stats::SampleP s{};  // crea sample
    std::for_each(v.cbegin(), v.cend(), [&s](exit_point const& p) {
      s.add(p);
    });  // aggiungi ogni elemento al sample

    stats::Statistics stats = s.statistics();  // calcoli statistici
    // stampa i dati
    output << "Distribuzione YF\n";
    output << "Mean: " << stats.mean.y << "\n"
           << "Sigma: " << stats.sigma.y << "\n"
           << "Mean_err: " << stats.mean_err.y << "\n"
           << "Skewness: " << stats.skewness.y << "\n"
           << "Kurtosis: " << stats.kurtosis.y << "\n";
  }

  output << "\n-------------\n";

  // calcoli statistici - ANGOLO DI USCITA
  /*{
    stats::Sample1D s{};  // crea sample

    std::for_each(v.cbegin(), v.cend(), [&s](exit_point const& p) {
      s.add(p.theta);
    });  // aggiungi ogni elemento al sample

    stats::Statistics stats = s.statistics();  // calcoli statistici
    // stampa i dati
    output << "Distribuzione ThetaF\n";
    output << "Mean: " << stats.mean << "\n"
           << "Sigma: " << stats.sigma << "\n"
           << "Mean_err: " << stats.mean_err << "\n"
           << "Skewness: " << stats.skewness << "\n"
           << "Kurtosis: " << stats.kurtosis << "\n";
  }*/
}

}  // namespace particleSimulator