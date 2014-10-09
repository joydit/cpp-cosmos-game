#include <string>
#include <Engine.h>
#include "Clients.h"
#include "Client.h"
#include "Network.h"
#include "Players.h"

using namespace std;

///Public
Clients::Clients(): clientIdCount(1), authDistribution(1, std::numeric_limits<Client::auth_id_t>::max())
{

}

Clients::~Clients()
{
   kickAll();
}

Client* Clients::findById(BaseClient::id_t clientId)
{
   find_if(all.begin(), all.end(), [clientId](Client*& client) {
      return client->getId() == clientId;
   });

   return NULL;
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
   find_if(all.begin(), all.end(), [username](Client*& client) {
      return client->getUsername() == username;
   });

   return NULL;
}

void Clients::kickAll()
{
   for (Client* client: getAll())
   {
      client->kick();
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
   GetEngine().clients.add(client);
   client->handleConnect();
}

void Clients::handleDisconnect(Client* client)
{
   GetEngine().clients.remove(client);
   client->handleDisconnect();
}
