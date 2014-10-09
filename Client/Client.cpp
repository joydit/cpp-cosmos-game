#include <Engine.h>
#include "Client.h"

using namespace std;

///Public
void Client::RecInitialRespond(Client& client, sf::Packet packet)
{
   int8_t status;

   packet >> status;

   if (status)
      Console::PrintInfo("Accepted");
}

void Client::RecInitialise(Client& client, sf::Packet packet)
{
   GetEngine().network.messages.unpackInitialise(packet);
}

void Client::RecUdpCheck(Client& client, sf::Packet packet)
{
   if (!client.canUseUdp()) ///Temporary - Add check of verification code
   {
      client.setUseUdp(true);
      Console::PrintInfo("Can use udp.");
   }
}

Client::Client(): BaseClient(new sf::TcpSocket, 0, 0, "")
{

}

Client::~Client()
{

}

void Client::sendInitial(username_t username, std::string password)
{
   sf::Packet packet = GetEngine().network.messages.create(Messages::Client::INITIAL);
   packet << username;
   packet << password;
   getSocket().send(packet);
}
