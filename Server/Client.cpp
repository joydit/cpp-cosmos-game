#include <Engine.h>
#include "Client.h"
#include "Network.h"
#include "Player.h"
#include "Clients.h"

using namespace std;

///Public
void Client::RecInitial(Client& client, sf::Packet packet)
{
   string username, password;
   packet >> username;
   packet >> password;

   client.setUsername(username);

   cout << "Login: " << username << " " << password << endl; ///Temporary

   SendInitialRespond(client);
   SendInitialise(client);
}

void Client::RecInitialise(Client& client, sf::Packet packet)
{

}

void Client::SendInitialRespond(Client& client)
{
   sf::Packet packet = GetEngine().network.messages.create(Messages::Server::INITIAL_RESPOND);
   packet << (int8_t)1; //Accepted (unfinished)
   client.getSocket().send(packet);

   ///Temporary - Use udp
   /*packet = GetEngine().network.messages.create(Messages::Server::UDP_CHECK);
   client.getSocket().send(packet);*/
}

void Client::SendInitialise(Client& client)
{
   sf::Packet packet = GetEngine().network.messages.create(Messages::Server::INITIALISE);
   GetEngine().network.messages.packInitialise(packet);
   client.getSocket().send(packet);
}

Client::Client(sf::TcpSocket* socketTcp, username_t username): BaseClient(socketTcp, 0, 0, username)
{
   cout << "Auth id: " << getAuthId() << endl; ///Temporary
}

Client::~Client()
{

}

bool Client::hasFinalised()
{
   return getState() == STATE_FINALISED;
}

Client::state_e Client::getState()
{
   return state;
}

string Client::getIpAddress()
{
   return getSocket().getRemoteAddress().toString();
}

void Client::kick()
{
   getSocket().disconnect();
   GetEngine().clients.handleDisconnect(this);
}

bool Client::verifyAuth(id_t clientId, auth_id_t authId)
{
   return BaseClient::verifyAuth(clientId, authId);
}

void Client::init()
{

}

void Client::handleConnect()
{
   Console::PrintInfo("Client Connected - Address: %s CID: #%u Count: %u\n", getIpAddress().c_str(), getId(), GetEngine().network.getOnlineCount());
}

void Client::handleDisconnect()
{
   Console::PrintInfo("Client Disconnected.");
}

///Protected
void Client::init(string name, string pass)
{
   setPlayer(new Player(this, name));
}

void Client::validate(string name, string pass)
{

}

void Client::finalise(bool valid)
{

}

///Private
void Client::setId(id_t clientId)
{
   BaseClient::setId(clientId);
}

void Client::setAuthId(auth_id_t authId)
{
   BaseClient::setAuthId(authId);
}
