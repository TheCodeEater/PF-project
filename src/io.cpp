#include "../include/io.hpp"

namespace particleSimulator {
options config::getOptions() const {
  return {2200, 1230,         "Simulatore di particelle", 400, 200, 700,
          300,  std::atanf(2)};
}
}  // namespace particleSimulator