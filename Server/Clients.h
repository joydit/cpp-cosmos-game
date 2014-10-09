/*
   Maintains a list of all the clients and provides find facilities.
*/

#pragma once

#include "../BaseClients.h"
#include "../BaseClient.h"

class Client;
class Network;

class Clients: public BaseClients
{
   friend class Client;
   friend class Network;

public:
   Clients();
   ~Clients();
   Client* findById(BaseClient::id_t clientId);
   Client* findByUniqueId(BasePlayer::unique_id_t uniqueId);
   Client* findByUsername(std::string username);
   void kickAll();

protected:
   BaseClient::id_t clientIdCount;
   std::default_random_engine authGenerator;
   std::uniform_int_distribution<BaseClient::auth_id_t> authDistribution;

   void add(Client* client);
   BaseClient::auth_id_t genAuthId();
   BaseClient::id_t genId();

   void handleConnect(Client* client);
   void handleDisconnect(Client* client);
};
