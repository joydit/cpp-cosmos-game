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

   static const port_t PORT_TCP_DEFAULT = 2500;
   static const port_t PORT_UDP_DEFAULT = 2600;

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
