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
   enum state_e { STATE_UNINITIALISED, STATE_PENDING, STATE_FINALISED };

   static void RecInitial(Client& client, sf::Packet packet);
   static void RecInitialise(Client& client, sf::Packet packet);
   static void SendInitialRespond(Client& client);
   static void SendInitialise(Client& client);

   Client(sf::TcpSocket* socketTcp);
   ~Client();
   bool hasFinalised();
   state_e getState();
   std::string getIpAddress();
   void kick(std::string reason = "");
   bool verifyAuth(id_t clientId, auth_id_t authId);
   void handleConnect();
   void handleDisconnect();
   void disconnect(std::string msg);
   void disconnect();

   void sendInitial();
   void sendDisconnectMessage(std::string msg);

protected:
   void init();
   void validate(std::string username, std::string password); //Check they are who they say they are
   void validateRespond(bool valid); //Result of check

private:
   state_e state;
   std::string validatePassword;

   void setId(id_t clientId);
   void setAuthId(auth_id_t authId);
};
