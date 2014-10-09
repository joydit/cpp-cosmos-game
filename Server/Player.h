#pragma once

#include "../BasePlayer.h"

class Client;

class Player: public BasePlayer
{
public:
   Player(Client* client, name_t name);
   bool isOnline();
   bool hasClient();
   Client& getClient();

private:
   Client* client;

   void setClient(Client* client);
};
