#include "../include/application.hpp"

namespace particleSimulator {
Application::Application(options const& opt)
    : optn_{opt},
      w_{sf::VideoMode(optn_.w_width, optn_.w_height), optn_.w_name},
      camera_{{optn_.w_width / 2.f - 30, 0}, {optn_.w_width, -optn_.w_height}},
      x_{sf::Lines, 2},
      y_{sf::Lines, 2} {
  w_.setView(camera_);  // set the current view

  //set x and y axis vertex position
  x_[0].position = {0, 0};
  x_[1].position = {optn_.w_width, 0};

  y_[1].position = {0, -optn_.w_height / 2.f};
  y_[0].position = {0, optn_.w_height / 2.f};

  //set color
  x_[0].color = sf::Color::Green;
  x_[1].color = sf::Color::Green;
  y_[0].color = sf::Color::Green;
  y_[1].color = sf::Color::Green;
}

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
    w_.draw(x_);
    w_.draw(y_);

    // display
    w_.display();
  }

  return 0;
}
}  // namespace particleSimulator