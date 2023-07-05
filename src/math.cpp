#include "../include/math.hpp"

#include <cmath>
#include "boost/math/constants/constants.hpp"

namespace bm = boost::math::constants;

namespace particleSimulator{
    float arctan(float y, float x) {
  const float theta = std::atan2f(y, x);
  // correggi l'angolo risultante in modo tale che sia compreso tra zero e
  // 2pi, non tra -pi e +pi
  if (theta < 0) {
    return theta + 2 * bm::pi<float>();
  } else {
    return theta;
  }
}
}