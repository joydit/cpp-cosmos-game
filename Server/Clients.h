/*
   Maintains a list of all the clients and provides find facilities.
*/

#pragma once

#include <Singleton.h>
#include "../BaseClients.h"
#include "../BaseClient.h"

class Client;

class Clients: public BaseClients, public Singleton<Clients>
{
   friend class Client;

public:
   enum filter_t { FILTER_ID, FILTER_UNIQUE_ID, FILTER_USERNAME };

   Clients();
   ~Clients();
   Client* findById(BaseClient::id_t clientId);
   Client* findByUniqueId(BasePlayer::unique_id_t uniqueId);
   Client* findByUsername(std::string username);
   Client* findByFilter(filter_t filter, std::string identifier);
   Client* findByString(std::string filter, std::string identifier);
   void disconnectAll(std::string reason);

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
