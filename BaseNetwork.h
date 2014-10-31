/*
   Handles the connecting and receiving of messages.
*/

#pragma once

#include <SFML/Network.hpp>
#include "Shared/Messages.h"

class Client;

class BaseNetwork
{
public:
   typedef uint16_t port_t;

   Messages messages;

   BaseNetwork();
   void setPortTcp(port_t portTcp);
   port_t getPortTcp();
   void setPortUdp(port_t portUdp);
   port_t getPortUdp();
   sf::IpAddress getLocalAddress();
   sf::IpAddress getPublicAddress();

protected:
   void handleClientReceivePacket(Client& client, sf::Packet& packet);

private:
   port_t portTcp, portUdp;
};
