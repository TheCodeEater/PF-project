#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>
#include <type_traits>

#include "math.hpp"
#include "trajectory.hpp"

namespace particleSimulator {
namespace stats {

 //valuta se il tipo passato come pramatro è usabile o meno con statistics
template<typename T>
constexpr bool is_statistic_allowed(){
  return std::is_arithmetic_v<T> || std::is_same_v<T,exit_point>;
}

template <typename T>
// struct dei momenti calcolati per la distribuzione del campione
struct Statistics {
  static_assert(is_statistic_allowed<T>());
  T mean{};
  T sigma{};
  T mean_err{};
  T skewness{};
  T kurtosis{};

  // costruisci la struct a partire dai momenti iniziali
  Statistics(T mean, T mean2, T mean3, T mean4, int N)
      : mean{mean},
        sigma{getSQRT(
            N / (N - 1.f) *
            (mean2 - mean * mean))},  // x quadro medio meno x medio quadro
        mean_err{sigma / std::sqrt(N)},
        skewness{(mean3 - 3 * mean * mean2 + 2 * mean * mean * mean) /
                 (sigma * sigma * sigma)},  //
        kurtosis{(mean4 - 4 * mean3 * mean + 6 * mean2 * mean * mean -
                  3 * mean * mean * mean * mean) /
                     (sigma * sigma * sigma * sigma) -
                 3} {}
};
template <typename T>
class Sample {
  static_assert(is_statistic_allowed<T>());
  std::vector<T> entries_{};  // vettore di punti sperimentali

  struct Accumulator;
  struct InitialMoments {
    T mean{};
    T mean2{};
    T mean3{};
    T mean4{};

    // costruisci la struct, eseguendo il calcolo di momenti iniziali
    InitialMoments(Accumulator const& acc, int N)
        : mean{acc.sum / N},
          mean2{acc.sum2 / N},
          mean3{acc.sum3 / N},
          mean4{acc.sum4 / N} {
      assert(N > 0);
    }
  };
  struct Accumulator {
    T sum{};
    T sum2{};
    T sum3{};
    T sum4{};

    Accumulator& operator+=(T const& value) {  // operatore di accumulo
      // esegui l'accumulo con gli operatori del tipo che usi
      sum += value;
      sum2 += value * value;
      sum3 += value * value * value;
      sum4 += value * value * value * value;

      return *this;
    }

    Accumulator operator+(T const& value) {
      Accumulator acc_sum{*this};
      acc_sum += value;
      return acc_sum;
    }

    InitialMoments getMoments(int N) {
      assert(N > 0);
      return InitialMoments{*this, N};
    }
  };

 public:
  // constructors
  Sample() = default;  // default constructor

  void add(T x) { entries_.push_back(x); }  // aggiungi dati al campione

  int size() const { return entries_.size(); }  // restituisci la dimensioni

  // rimuovi dato e restituisci se la rimozione è stata effettuata o no
  // (nel qual caso il punto rimosso non vi era)
  bool remove(T val) {
    auto it = std::find(entries_.begin(), entries_.end(),
                        val);    // cerca valore da rimuovere
    if (it != entries_.end()) {  // se lo trovi, cancellalo
      entries_.erase(it);
      return true;
    } else {  // altrimenti, non fare nulla e restituisci che non lo hai trovato
      return false;
    }
  }

  Statistics<T> statistics() const {
    int N_ = entries_.size();
    if (N_ == 0) {
      throw std::runtime_error(  // check delle condizioni iniziali
          "Cannot calculate parameters from empty sample!");
    }
    // accumulatori di somma.
    const Accumulator sums{std::accumulate(entries_.begin(), entries_.end(),
                                           Accumulator{})};  // esegui le somme

    const InitialMoments imoments{sums, N_};

    return Statistics{imoments.mean, imoments.mean2, imoments.mean3,
                      imoments.mean4, N_};
  }

  const auto& entries() const {
    return entries_;
  }  // funzione per accedere al vettore in caso di necessità ma senza
     // modificarlo

  Sample& operator+=(const Sample& rhs) {
    entries_.insert(entries_.end(), rhs.entries().begin(),
                    rhs.entries().end());  // inserisci la sequenza di dati di
                                           // rhs alla fine del vettore di lhs
    return *this;
  }
};

template <typename T>
Sample<T> operator+(
    Sample<T> const& l,
    Sample<T> const& r) {  
      static_assert(is_statistic_allowed<T>());
      // analogo ma con operatore + invece di +=
  Sample s{l};             // copia di sample
  s += r;
  return s;
}

}  // namespace stats

}  // namespace particleSimulator

#endif