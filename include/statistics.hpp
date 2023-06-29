#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>

namespace stats {

class Sample;
struct Statistics;

class Sample {
 public:
 using T=double;
  //constructors
  Sample()=default;

  void add(T);
  int size() const;
  bool remove(T);

  Statistics statistics() const;

  const auto& entries() const { return entries_; }

  Sample& operator+=(const Sample&);

   private:
  std::vector<T> entries_{};
};

struct Statistics {
  Sample::T mean{};
  Sample::T sigma{};
  Sample::T mean_err{};
  Sample::T kurtosis{};
  Sample::T skewness{};
};


Sample operator+(Sample const& l, Sample const& r);

}  // namespace pf

#endif