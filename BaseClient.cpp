#include "BaseClient.h"

using namespace std;

///Public
BaseClient::BaseClient(sf::TcpSocket* socket, id_t clientId, auth_id_t authId, username_t username): socket(socket), useUdp(false),  player(NULL)
{
   setUsername(username);
   setId(clientId);
   setAuthId(authId);
}

BaseClient::~BaseClient()
{
   delete socket; //Client's are only created with a valid TCP socket
}

BaseClient::id_t BaseClient::getId()
{
   return clientId;
}

BaseClient::auth_id_t BaseClient::getAuthId()
{
   return authId;
}

BaseClient::username_t BaseClient::getUsername()
{
   return username;
}

sf::TcpSocket& BaseClient::getSocket()
{
   return *socket;
}

bool BaseClient::canUseUdp()
{
   return useUdp;
}

void BaseClient::setId(id_t clientId)
{
   this->clientId = clientId;
}

void BaseClient::setAuthId(auth_id_t authId)
{
   this->authId = authId;
}

bool BaseClient::verifyAuth(id_t clientId, auth_id_t authId)
{
   return clientId == getId() && authId == getAuthId();
}

void BaseClient::init()
{

}

void BaseClient::handleConnect()
{

}

void BaseClient::handleDisconnect()
{

}

///Protected
void BaseClient::setPlayer(player_t player)
{
   this->player = player;
}

void BaseClient::setUseUdp(bool enable)
{
   useUdp = enable;
}

void BaseClient::setUsername(username_t username)
{
   this->username = username;
}
