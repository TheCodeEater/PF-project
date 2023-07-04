#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "graphics.hpp"
#include "io.hpp"
#include "trajectory.hpp"

namespace particleSimulator {

// alias for sfml render window
using W = sf::RenderWindow;

class Application {
  const options optn_{};  // opzioni simulazione
  W w_;                   // sfml render window
  sf::View camera_{};     // sfml view per inquadrare la simulazione

  // graphic objects
  const sf::VertexArray x_{};  // asse x
  const sf::VertexArray y_{};  // asse y

  const sf::VertexArray line_sup_{};  // bordo sup
  const sf::VertexArray line_inf_{};  // bordo inf

  // logic objects
  const simulation simulation_;  // oggetto di simulazione
  particle particle_{};          // particella studiata
  std::pair<std::vector<dottedLine>, exit_point>
      result_{};  // segmenti traiettoria e punto di uscita

 public:
  explicit Application(options const&);

  exit_point loop();  // loop applicazione
};

}  // namespace particleSimulator

#endif