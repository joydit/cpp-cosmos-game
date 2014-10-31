#pragma once

#include <SFML/Network.hpp>

class Player;

class BaseClient
{
public:
   typedef uint32_t id_t;
   typedef uint32_t auth_id_t;
   typedef std::string username_t;
   typedef Player* player_t;

   BaseClient(sf::TcpSocket* socket, id_t clientId, auth_id_t authId, username_t username);
   ~BaseClient();
   id_t getId();
   auth_id_t getAuthId();
   username_t getUsername();
   player_t getPlayer();
   sf::TcpSocket& getSocket();
   bool canUseUdp();
   virtual void setId(id_t clientId);
   virtual void setAuthId(auth_id_t authId);
   virtual bool verifyAuth(id_t clientId, auth_id_t authId);

   virtual void init();
   virtual void disconnect();
   virtual void handleConnect();
   virtual void handleDisconnect();

protected:
   void setPlayer(player_t player);
   void setUseUdp(bool enable); //(Server->Client) Automatically checks for port forwarded users, override through config
   void setUsername(username_t username);

private:
   id_t clientId;
   auth_id_t authId; //Random value only the client and server knows, avoids people sending packets pretending to be someone else
   player_t player;
   username_t username;
   sf::TcpSocket* socket;
   bool useUdp;
};
