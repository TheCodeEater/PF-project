#include "../include/application.hpp"

namespace particleSimulator {
Application::Application(options const& opt)
    : optn_{opt},
      w_{sf::VideoMode(optn_.w_width, optn_.w_height), optn_.w_name} {}

int Application::loop() {
  // run the program as long as the window is open
  while (w_.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event e;
    while (w_.pollEvent(e)) {
      // event handling
      switch (e.type) {
        case sf::Event::Closed:
          w_.close();
          break;

        default:
          // ignore
          break;
      }
    }

    // clear window. set black color
    w_.clear(sf::Color::Black);

    // draw everything here...
    // window.draw(...);

    // display
    w_.display();
  }

  return 0;
}
}  // namespace particleSimulator