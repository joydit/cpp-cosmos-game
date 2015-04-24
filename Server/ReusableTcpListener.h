#include <SFML/Network.hpp>

class ReusableTcpListener : public sf::TcpListener {
public:
  void free();
};
