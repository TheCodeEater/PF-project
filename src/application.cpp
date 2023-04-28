#include "../include/application.hpp"

namespace particleSimulator {
Application::Application(options const& opt)
    : optn_{opt},
      w_{sf::VideoMode(optn_.w_width, optn_.w_height), optn_.w_name} {}
}  // namespace particleSimulator