#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <string>
#include <vector>

#include "graphics.hpp"
#include "traiettoria.hpp"
#include "io.hpp"

namespace particleSimulator {

// alias for sfml render window
using W = sf::RenderWindow;

class Application {
  const options optn_{};  // window options
  W w_;                   // sfml render window
  sf::View camera_{};     // view

  // graphic objects
  const sf::VertexArray x_{};  // x axis
  const sf::VertexArray y_{};  // y axis

  const sf::VertexArray line_sup_{};
  const sf::VertexArray line_inf_{};

  // logic objects
  const simulation simulation_;             // simulation object
  particle particle_{};                     // studied particle
  std::vector<dottedLine> trajectories_{};  // trajecotry history

 public:
  explicit Application(options const&);

  // likely to be removed
  ~Application() = default;  // default destructir
  // disable copy and move
  Application(Application const&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application const&) = delete;
  Application& operator=(Application&&) = delete;

  particle loop();  // run application loop
};

}  // namespace particleSimulator

#endif