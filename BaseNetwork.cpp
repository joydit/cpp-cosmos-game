#include "BaseNetwork.h"
#include "Shared/Client.h"

using namespace std;

///Public
BaseNetwork::BaseNetwork()
{
   setPortTcp(PORT_TCP_DEFAULT);
   setPortUdp(PORT_UDP_DEFAULT);
}

void BaseNetwork::setPortTcp(port_t portTcp)
{
   this->portTcp = portTcp;
}

BaseNetwork::port_t BaseNetwork::getPortTcp()
{
   return portTcp;
}

void BaseNetwork::setPortUdp(port_t portUdp)
{
   this->portUdp = portUdp;
}

BaseNetwork::port_t BaseNetwork::getPortUdp()
{
   return portUdp;
}

sf::IpAddress BaseNetwork::getLocalAddress()
{
   return sf::IpAddress::getLocalAddress();
}

sf::IpAddress BaseNetwork::getPublicAddress()
{
   return sf::IpAddress::getPublicAddress();
}

///Protected
void BaseNetwork::handleClientReceivePacket(Client& client, sf::Packet& packet)
{
   Messages::id_t msgId;
   packet >> msgId;

   if (messages.receivable.getAll().in_bounds(msgId))
      messages.receivable.get(msgId).call<Client&>(client, packet);
   else
      Console::PrintError("Packet id unknown '%i'", msgId);
}
