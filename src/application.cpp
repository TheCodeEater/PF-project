#include "../include/application.hpp"

#include "../include/traiettoria.hpp"

#include "../include/graphics.hpp"

#include <iostream>

namespace particleSimulator {
Application::Application(options const& opt)
    : optn_{opt},
      w_{sf::VideoMode(optn_.w_width, optn_.w_height), optn_.w_name},
      camera_{{optn_.w_width / 2.f - 30, 0}, {optn_.w_width, -optn_.w_height}},
      x_{[this]() {
        // create X axis
        sf::VertexArray x_axis{sf::Lines, 2};

        // set position
        x_axis[0].position = {0, 0};
        x_axis[1].position = {optn_.w_width, 0};

        // set color
        x_axis[0].color = sf::Color::Green;
        x_axis[1].color = sf::Color::Green;

        return x_axis;
      }()},  // created and run lambda
      y_{[this]() {
        // create Y axis
        sf::VertexArray y_axis{sf::Lines, 2};

        // set position
        y_axis[1].position = {0, -optn_.w_height / 2.f};
        y_axis[0].position = {0, optn_.w_height / 2.f};

        // set color
        y_axis[0].color = sf::Color::Green;
        y_axis[1].color = sf::Color::Green;

        return y_axis;
      }()},
      line_sup_{[this]() {
        // create upper rect
        sf::VertexArray l_sup{sf::Lines, 2};

        // set coordinates
        l_sup[0].position = {0, optn_.r1};
        l_sup[1].position = {optn_.l, optn_.r2};

        l_sup[0].color = sf::Color::Red;
        l_sup[1].color = sf::Color::Red;

        return l_sup;
      }()},
      line_inf_{[this]() {
        // create upper rect
        sf::VertexArray l_inf{sf::Lines, 2};

        // set coordinates
        l_inf[0].position = {0, -optn_.r1};
        l_inf[1].position = {optn_.l, -optn_.r2};

        l_inf[0].color = sf::Color::Red;
        l_inf[1].color = sf::Color::Red;

        return l_inf;
      }()} {
  w_.setView(camera_);  // set the current view
}

int Application::loop() {
  particleSimulator::path biliardo{optn_.r1,optn_.r2,optn_.l};//inizializza biliardo
  //esegui il calcolo della traiettoria
  particleSimulator::particle p0{{0,0}, std::atan(1)};
  auto p1=p0;

  std::vector<particleSimulator::dottedLine> trajs{};

  for(int i{};i<4;i++){
    std::cout<<"It: "<<i<<"\n";
    std::cout<<"Angolo nuova traiettoria: "<<biliardo.reflect(p0); //calcola la riflessione
    particleSimulator::dottedLine line{{p1.pos.x(),p1.pos.y()},{p0.pos.x(),p0.pos.y()}};
    trajs.push_back(line);
    p1=p0;
  }

  std::cout<<"Angolo ultima iterazione: "<<p0.theta<<"\n";

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
    w_.draw(line_inf_);
    w_.draw(line_sup_);
    //
    for(auto const& l:trajs){//disegna le traiettorie
      l.draw(w_);
    }
    // display
    w_.display();
  }

  return 0;
}
}  // namespace particleSimulator