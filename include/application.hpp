#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Graphics.hpp>
#include <string>

namespace particleSimulator{

//alias for sfml render window
using W=sf::RenderWindow;

class Application{
    //simulation options
    struct options{
        int w_width{};
        int w_height{};
        std::string w_name{};
    };

    options optn_{};
    W w_;//sfml render window

    public:
        Application(options const&);

        ~Application()=default; //default destructir
        //disable copy and move
        Application(Application const&)=delete;
        Application(Application&&)=delete;
        Application& operator=(Application const&)=delete;
        Application& operator=(Application&&)=delete;

        int loop(); //run application loop

};

}


#endif