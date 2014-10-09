#pragma once

#include <list>
#include "BasePlayer.h"

class Client;

class BaseClients
{
public:
   typedef std::list<Client*> clients_t;

   BaseClients();
   ~BaseClients();
   int getCount();
   clients_t getAll();

protected:
   clients_t all;
   clients_t removed;

   void add(Client* client);
   void remove(Client* client);
   void cleanup();
};
