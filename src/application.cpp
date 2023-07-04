#include "../include/application.hpp"

#include <iostream>

#include "../include/graphics.hpp"

namespace particleSimulator {
Application::Application(options const& opt)
    : optn_{opt},
      w_{sf::VideoMode(optn_.w_width, optn_.w_height),
         optn_.w_name},  // configura la finestra
      camera_{{optn_.w_width / 2.f - 30, 0},
              {optn_.w_width,
               -optn_.w_height}},  // inquadra il biliardo, invertendo asse Y e
                                   // collocando (0,0) al centro a sinistra
      x_{[this]() {  // crea ed esegui lambda: necessario per poter eseguire
                     // operazioni sull'oggetto, dato che essendo il data member
                     // const una volta iniailizzato non lo posso modificare
        // se invece fosse const non il membro dato ma tutto l'oggetto, potrei
        // svolgere questo pezzo nel codice del costruttore
        //  crea asse X
        sf::VertexArray x_axis{sf::Lines, 2};

        // Imposta veritici
        x_axis[0].position = {0, 0};
        x_axis[1].position = {optn_.w_width, 0};

        // imposta colori
        x_axis[0].color = sf::Color::Green;
        x_axis[1].color = sf::Color::Green;

        return x_axis;
      }()},  // esegui la lambda
      y_{[this]() {
        // crea asse Y
        sf::VertexArray y_axis{sf::Lines, 2};

        // imposta vertii
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
      }()},
      simulation_{optn_.r1, optn_.r2, optn_.l, optn_.N},
      particle_{{0, optn_.y0}, optn_.theta0} {
  // test sulle condizioni iniziali della particella

  assert(std::abs(particle_.pos.x()) < path::eps);  // coordinata X=0
  // coordinata Y tra -r1 e r1
  assert(particle_.pos.y() <= optn_.r1 + path::eps);
  assert(particle_.pos.y() >= -optn_.r1 - path::eps);
  // test sull'angolo: per l'input, tra -pi/2 e pi/2
  assert(std::abs(particle_.theta) < bm::pi<float>() / 2 + path::eps);

  // normalizzazione tra 0 e 2pi
  particle_.theta = (particle_.theta < 0)
                        ? particle_.theta + 2 * bm::pi<float>()
                        : particle_.theta;

  w_.setView(camera_);  // imposta la view creata
}

exit_point Application::loop() {  // esegui il game loop
  // rcalcolo traiettoria
  result_ = simulation_(particle_);
  std::vector<dottedLine> const& trajectories = result_.first;

  // gira finché la finestra è aperta
  while (w_.isOpen()) {
    // controlla gli eventi
    sf::Event e;
    while (w_.pollEvent(e)) {
      // event handling
      switch (e.type) {
        case sf::Event::Closed:  // chiudi se si clicca la X rossa
          w_.close();
          break;

        default:
          // ignora altri eventi
          break;
      }
    }

    // pulisci la finestra
    w_.clear(sf::Color::Black);

    // disegna gli elementi del biliardo
    w_.draw(x_);
    w_.draw(y_);
    w_.draw(line_inf_);
    w_.draw(line_sup_);

    // disegna le traiettorie
    std::for_each(trajectories.cbegin(), trajectories.cend(),
                  [this](dottedLine const& line) {
                    line.draw(w_);  // draw line on the current window
                  });
    // display
    w_.display();
  }

  return result_.second;  // restituisci il punto di uscita
}
}  // namespace particleSimulator