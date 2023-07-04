#include "../include/statistics.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>

namespace particleSimulator {
namespace stats {
void Sample::add(T x) { entries_.push_back(x); }

int Sample::size() const { return entries_.size(); }

bool Sample::remove(T val) {
  auto it = std::find(entries_.begin(), entries_.end(),
                      val);    // cerca valore da rimuovere
  if (it != entries_.end()) {  // se lo trovi, cancellalo
    entries_.erase(it);
    return true;
  } else {  // altrimenti, non fare nulla e restituisci che non lo hai trovato
    return false;
  }
}
Statistics Sample::statistics() const {
  int N_ = entries_.size();
  if (N_ == 0) {
    throw std::runtime_error(  // check delle condizioni iniziali
        "Cannot calculate parameters from empty sample!");
  }
  // accumulatori di somma.
  T sum_{};
  T sum2_{};
  T sum3_{};
  T sum4_{};
  std::for_each(entries_.begin(), entries_.end(),
                [&sum_, &sum2_, &sum3_, &sum4_](T val) {  // calcola le somme
                  sum_ += val;
                  sum2_ += val * val;
                  sum3_ += val * val * val;
                  sum4_ += val * val * val * val;
                });

  // calcola momenti iniziali (initial moments)
  const T mean{sum_ / N_};
  const T mean2{sum2_ / N_};
  const T mean3{sum3_ / N_};
  const T mean4{sum4_ / N_};

  // momenti centrali
  const T sigma{
      sqrt(N_ / (N_ - 1) *
           (mean2 - mean * mean))};  // x quadro medio meno x medio quadro
  const T mean_err = {sigma / sqrt(N_)};

  const T skewness{(mean3 - 3 * mean * mean2 + 2 * mean * mean * mean) /
                   (sigma * sigma * sigma)};  //

  const T kurtosis{(mean4 - 4 * mean3 * mean + 6 * mean2 * mean * mean -
                    3 * mean * mean * mean * mean) /
                       (sigma * sigma * sigma * sigma) -
                   3};

  return {mean, sigma, mean_err, kurtosis, skewness};
}

Sample& Sample::operator+=(const Sample& rhs) {
  entries_.insert(entries_.end(), rhs.entries().begin(),
                  rhs.entries().end());  // inserisci la sequenza di dati di rhs
                                         // alla fine del vettore di lhs
  return *this;
}

Sample operator+(Sample const& l,
                 Sample const& r) {  // analogo ma con operatore + invece di +=
  Sample s{l};                       // copia di sample
  s += r;
  return s;
}
}  // namespace stats

}  // namespace particleSimulator