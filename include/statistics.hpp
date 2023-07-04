#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>

namespace particleSimulator {
namespace stats {

struct Statistics;  // forward declaration per poterla usare della definizione
                    // di sample

class Sample {
 public:
  using T = double;  // necessario pocihé le funzioni matematiche restituiscono
                     // in double
  // constructors
  Sample() = default;  // default constructor

  void add(T);       // aggiungi dati al campione
  int size() const;  // restituisci la dimensioni
  bool remove(
      T);  // rimuovi dato e restituisci se la rimozione è stata effettuata o no
           // (nel qual caso il punto rimosso non vi era)

  Statistics statistics() const;  // calcoli statistici

  const auto& entries() const {
    return entries_;
  }  // funzione per accedere al vettore in caso di necessità ma senza
     // modificarlo

  Sample& operator+=(const Sample&);  // unisci due campioni

 private:
  std::vector<T> entries_{};  // vettore di punti sperimentali
};

struct Statistics {  // struct dei momenti calcolati per la distribuzione del
                     // campnione
  Sample::T mean{};
  Sample::T sigma{};
  Sample::T mean_err{};
  Sample::T kurtosis{};
  Sample::T skewness{};
};

Sample operator+(Sample const& l, Sample const& r);  // unisci campioni

}  // namespace stats

}  // namespace particleSimulator

#endif