#include "../include/application.hpp"
#include "../include/io.hpp"

// alias for namespace
namespace ps = particleSimulator;

int main() {
  ps::config c{};
  ps::Application app{c.getOptions()};

  return app.loop();
}