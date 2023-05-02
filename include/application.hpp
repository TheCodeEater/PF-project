#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Graphics.hpp>
#include <string>

namespace particleSimulator {

// alias for sfml render window
using W = sf::RenderWindow;

// simulation options
struct options {
  float w_width{};
  float w_height{};
  std::string w_name{};
};

class Application {
  const options optn_{};  // window options
  W w_;                   // sfml render window
  sf::View camera_{};     // view

  // graphic objects
  const sf::VertexArray x_{};  // x axis
  const sf::VertexArray y_{};  // y axis

 public:
  explicit Application(options const&);

  //likely to be removed
  ~Application() = default;  // default destructir
  // disable copy and move
  Application(Application const&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application const&) = delete;
  Application& operator=(Application&&) = delete;

  int loop();  // run application loop
};

}  // namespace particleSimulator

#endif