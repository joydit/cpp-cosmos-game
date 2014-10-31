#include <string>
#include <Engine.h>
#include "Clients.h"
#include "Client.h"
#include "Players.h"

using namespace std;

///Public
Clients::Clients(): clientIdCount(1), authDistribution(1, std::numeric_limits<Client::auth_id_t>::max())
{

}

Clients::~Clients()
{
   for (Client* client: getAll())
      delete client;
}

Client* Clients::findById(BaseClient::id_t clientId)
{
   clients_t::iterator it = find_if(all.begin(), all.end(), [clientId](Client*& client) {
      return client->getId() == clientId;
   });

   if (it == all.end())
      return NULL;

   return *it;
}

Client* Clients::findByUniqueId(BasePlayer::unique_id_t uniqueId)
{
   Player* player = GetEngine().players.findByUniqueId(uniqueId, Players::FILTER_ONLINE);
   if (!player)
      return NULL;

   return &player->getClient();
}

Client* Clients::findByUsername(string username)
{
   clients_t::iterator it = find_if(all.begin(), all.end(), [username](Client*& client) {
      return client->getUsername() == username;
   });

   if (it == all.end())
      return NULL;

   return *it;
}

Client* Clients::findByFilter(filter_t filter, string identifier)
{
   if (filter == FILTER_USERNAME)
      return findByUsername(identifier);

   stringstream ss;
   Player::unique_id_t id;

   ss << identifier;
   ss >> id;

   switch(filter)
   {
   case FILTER_ID:
      return findById(id);

   case FILTER_UNIQUE_ID:
      return findByUniqueId(id);
   }

   return NULL;
}

Client* Clients::findByString(std::string filter, std::string identifier)
{
   if (filter == "cid" || filter == "clientid")
      return findByFilter(FILTER_ID, identifier);

   if (filter == "uid" || filter == "uniqueid")
      return findByFilter(FILTER_UNIQUE_ID, identifier);

   if (filter == "uname" || filter == "username")
      return findByFilter(FILTER_USERNAME, identifier);

   Console::PrintError("Invalid filter '%s'", filter.c_str());
   return NULL;
}

void Clients::disconnectAll(std::string reason)
{
   sf::Packet packet = Messages::Get().create(Messages::Server::DISCONNECT_DIALOGUE);
   packet << reason;

   for (Client* client: getAll())
   {
      client->getSocket().send(packet);
      client->disconnect();
   }
}

///Protected
void Clients::add(Client* client)
{
   client->setAuthId(genAuthId());
   client->setId(genId());

   BaseClients::add(client);
}

BaseClient::auth_id_t Clients::genAuthId()
{
   return authGenerator();
}

BaseClient::id_t Clients::genId()
{
   return (clientIdCount++ % std::numeric_limits<BaseClient::id_t>::max()); //Make it start at one
}

void Clients::handleConnect(Client* client)
{
   Network::Get().selector.add(client->getSocket());
   add(client);
}

void Clients::handleDisconnect(Client* client)
{
   remove(client);
   Network::Get().selector.remove(client->getSocket());
}
