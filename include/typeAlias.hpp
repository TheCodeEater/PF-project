#ifndef TYPE_ALIAS_HPP
#define TYPE_ALIAS_HPP

#include "statistics.hpp"
#include "trajectory.hpp"

namespace particleSimulator {
    namespace stats {
    // type alias
        using SampleP = Sample<exit_point>;  // alias per sample di particelle
        using Sample1D = Sample<double>;
    }  // namespace stats
}  // namespace particleSimulator

#endif