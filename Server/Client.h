/*
   Contains all client data including sockets, authentication and the handling of them.
*/

#pragma once

#include "../BaseClient.h"

class Clients;

class Client: public BaseClient
{
   friend class Clients;

public:
   enum state_e { STATE_UNINITIALISED, STATE_VALIDATING, STATE_FINALISED };

   static void RecInitial(Client& client, sf::Packet packet);
   static void RecInitialise(Client& client, sf::Packet packet);
   static void SendInitialRespond(Client& client);
   static void SendInitialise(Client& client);

   Client(sf::TcpSocket* socketTcp, username_t username);
   ~Client();
   bool hasFinalised();
   state_e getState();
   std::string getIpAddress();
   void kick();
   bool verifyAuth(id_t clientId, auth_id_t authId);
   void init();
   void handleConnect();
   void handleDisconnect();
   void sendInitial();

protected:
   void init(std::string name, std::string pass); //Give initial values
   void validate(std::string name, std::string pass); //Check they are who they say they are
   void finalise(bool valid); //Result of check

private:
   state_e state;

   void setId(id_t clientId);
   void setAuthId(auth_id_t authId);
};
