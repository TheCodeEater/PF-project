#ifndef IO_HPP
#define IO_HPP

#include "application.hpp"
namespace particleSimulator {

class config {
 public:
  config() = default;
  options getOptions() const;
  options getOptions(float y0, float theta0, int N) const;
};
}  // namespace particleSimulator

#endif