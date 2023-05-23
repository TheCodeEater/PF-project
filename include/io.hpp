#ifndef IO_HPP
#define IO_HPP

#include "application.hpp"
namespace particleSimulator {

class config {
 public:
  config() = default;
  options getOptions() const;
};
}  // namespace particleSimulator

#endif