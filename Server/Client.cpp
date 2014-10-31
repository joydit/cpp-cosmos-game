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
   sf::Packet packet = Network::Get().messages.create(Messages::Server::INITIAL_RESPOND);
   packet << (int8_t)1; //Accepted (unfinished)
   client.getSocket().send(packet);

   ///Temporary - Use udp
   /*packet = Network::Get().messages.create(Messages::Server::UDP_CHECK);
   client.getSocket().send(packet);*/
}

void Client::SendInitialise(Client& client)
{
   sf::Packet packet = Network::Get().messages.create(Messages::Server::INITIALISE);
   Network::Get().messages.packInitialise(packet);
   client.getSocket().send(packet);
}

Client::Client(sf::TcpSocket* socketTcp): BaseClient(socketTcp, 0, 0, "Pending"), state(STATE_UNINITIALISED)
{

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

void Client::kick(string reason)
{
   if (reason != "")
      reason = "You have been kicked for '" + reason + "'";
   else
      reason = "You have been kicked from the server.";

   disconnect(reason);
}

bool Client::verifyAuth(id_t clientId, auth_id_t authId)
{
   return BaseClient::verifyAuth(clientId, authId);
}

void Client::handleConnect()
{
   Console::PrintInfo("Client Connected - Address: %s CID: #%u Count: %u\n", getIpAddress().c_str(), getId(), Network::Get().getOnlineCount());

   Clients::Get().handleConnect(this);
}

void Client::handleDisconnect()
{
   getSocket().disconnect();
   Clients::Get().handleDisconnect(this);

   Console::PrintInfo("Client Disconnected.");
}

void Client::disconnect(string msg)
{
   sendDisconnectMessage(msg);

   BaseClient::disconnect();
}

void Client::disconnect()
{
   BaseClient::disconnect();
}

void Client::sendInitial()
{
   ///Temporary - Complete
}

void Client::sendDisconnectMessage(string msg)
{
   sf::Packet packet = Messages::Get().create(Messages::Server::DISCONNECT_DIALOGUE);
   packet << msg;
   getSocket().send(packet);
}

///Protected
void Client::init()
{
   setPlayer(new Player(this, getUsername()));
}

void Client::validate(string username, string password)
{

}

void Client::validateRespond(bool valid)
{
   validatePassword = "";

   if (valid)
   {

   }
   else
      disconnect();
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
