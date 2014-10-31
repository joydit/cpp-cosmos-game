#include "BaseClients.h"
#include "Shared/Engine.h"
#include "Shared/Network.h"
#include "Shared/Player.h"
#include "Shared/Client.h"

using namespace std;

///Public
BaseClients::BaseClients()
{

}

BaseClients::~BaseClients()
{

}

int BaseClients::getCount()
{
   return all.size();
}

BaseClients::clients_t BaseClients::getAll()
{
   return all;
}

///Private
void BaseClients::add(Client* client)
{
   all.push_back(client);
}

void BaseClients::remove(Client* client)
{
   auto it = std::find(all.begin(), all.end(), client);
   if (it != all.end())
      all.erase(it);

   removed.push_back(client);
}

void BaseClients::handleCleanup()
{
   for (Client*& client: removed)
   {
      delete client;
   }

   removed.clear();
}
