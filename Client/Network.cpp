#include <Engine.h>
#include "Network.h"
#include "Client.h"

using namespace std;

///Public
Network::Network()
{

}

Network::~Network()
{

}

bool Network::connect(sf::IpAddress address, port_t port)
{
   setServerAddress((address == "")? Config::Get().settings.get("network", "address", getLocalAddress()): address);
   setPortTcp((port == 0)? getPortTcp(): port);

   if (GetEngine().client.getSocket().connect(getServerAddress(), getPortTcp()) != sf::Socket::Done)
   {
      Console::PrintInfo("Failed to connect.");
      return false; //Failed to connect
   }

   GetEngine().client.getSocket().setBlocking(false);

   return true;
}

bool Network::update()
{
   sf::Packet packet;
   switch(GetEngine().client.getSocket().receive(packet))
   {
   case sf::Socket::Done: //Message
      handleClientReceivePacket(GetEngine().client, packet);
      break;

   case sf::Socket::Disconnected:
      Console::PrintInfo("Server disconnected.");
      return false;

   case sf::Socket::Error:
      Console::PrintError("Lost connection to server.");
      break;
   }

   return true;
}

///Private
void Network::setServerAddress(sf::IpAddress serverAddress)
{
   this->serverAddress = serverAddress;
}

sf::IpAddress Network::getServerAddress()
{
   return serverAddress;
}
