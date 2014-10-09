#pragma once

#include "../BaseNetwork.h"

class Client;

class Network: public BaseNetwork
{
public:
   Network();
   ~Network();
   bool connect(sf::IpAddress address = "", port_t port = 0);
   bool update();

private:
   sf::IpAddress serverAddress;

   void setServerAddress(sf::IpAddress serverAddress);
   sf::IpAddress getServerAddress();
};
