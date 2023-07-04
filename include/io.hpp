#ifndef IO_HPP
#define IO_HPP

#include <string>
#include <vector>

#include "../boost/property_tree/ini_parser.hpp"
#include "../boost/property_tree/ptree.hpp"
#include "traiettoria.hpp"
namespace particleSimulator {

// strutture dati opzioni di simulazione
struct options {
  float w_width{};
  float w_height{};
  std::string w_name{};
  // dati esperimento
  float r1{};
  float r2{};
  float l{};
  // dati particella
  float y0{};
  float theta0{};
  int N{};
};

struct randOptions {  // opzioni per simulazione random
  float r1{};
  float r2{};
  float l{};

  float pos_mean{};
  float pos_sigma{};
  float angle_mean{};
  float angle_sigma{};

  unsigned seed{};  // seme

  bool randomSeed{};  // se true, genera un nuovo seme ogni volta

  randOptions() = default;  // default constructor
  randOptions(float r1, float r2, float l, float p_mean, float p_sigma,
              float a_mean, float a_sigma, unsigned seed,
              bool genRSeed);  // aggregate constructor
  randOptions(randOptions const& rData,
              unsigned rSeed);  // costruttore con cambio di seme
};

class config {  // classe per caricare le impostazioni
  // impostazioni
  options wOptn_{};      // opzioni simulazione grafica
  randOptions rOptn_{};  // opzioni simulazione random
  int N_iter_{};         // numero di iterazioni massimo
  int N_particles_{};    // numero di particelle

 public:
  config();  // costruttore

  options getApplicationOptions(float y0, float theta0, int N)
      const;  // ottieni opzioni applicazione (by value poiché alcuni dati sono
              // aggiunti sul momento)
  randOptions getRandomOptions()
      const;  // ottieni opzione random (by value poiché alcuni dati sono
              // aggiunti al momento)
  int getIterations() const;      // restituisci numero di iterazioni massime
  int getParticleNumber() const;  // numero massimo di particelle

  // export dei dati su file
  void exportData(std::vector<exit_point> const& v,
                  std::string const& filename) const;
  void exportStatistics(std::vector<exit_point> const& v,
                        std::string const& filename) const;
};
}  // namespace particleSimulator

#endif