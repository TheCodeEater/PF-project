#include "../include/application.hpp"

namespace particleSimulator {
Application::Application(options const& opt)
    : optn_{opt},
      w_{sf::VideoMode(optn_.w_width, optn_.w_height), optn_.w_name} {}

int Application::loop(){
    sf::VertexArray x{sf::Lines,2};
    x[0].position={-optn_.w_width/2.f,0};
    x[1].position={optn_.w_width/2.f,0};
    x[0].color=sf::Color::Green;
    x[1].color=sf::Color::Green;
    sf::VertexArray y{sf::Lines,2};
    y[1].position={0,-optn_.w_height/2.f};
    y[0].position={0,optn_.w_height/2.f};
    y[0].color=sf::Color::Green;
    y[1].color=sf::Color::Green;


    sf::View view({optn_.w_width/2.f-30,0},{static_cast<float>(optn_.w_width),static_cast<float>(-optn_.w_height)});
    w_.setView(view);//set the view

    // run the program as long as the window is open
    while (w_.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event e;
        while (w_.pollEvent(e))
        {
            //event handling
            switch(e.type){
              case sf::Event::Closed:
                w_.close();
                break;

              default:
                //ignore
                break;
            }
        }

        //clear window. set black color
        w_.clear(sf::Color::Black);

        // draw everything here...
        w_.draw(x);
        w_.draw(y);

        // display
        w_.display();
    }

    return 0;
}
}  // namespace particleSimulator