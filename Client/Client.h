#pragma once

#include <Singleton.h>
#include "../BaseClient.h"

class Client: public BaseClient, Singleton <Client>
{
public:
   static void RecInitialRespond(Client& client, sf::Packet packet);
   static void RecInitialise(Client& client, sf::Packet packet);
   static void RecUdpCheck(Client& client, sf::Packet packet);
   static void RecDialogue(Client& client, sf::Packet packet);

   Client();
   ~Client();

   void sendInitial(username_t username, std::string password);
};
